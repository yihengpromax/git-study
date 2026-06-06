#include "stdafx.h"
#include "protocol.h"
#include "server.h"
#include "database.h"
#include "utils/utils.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QUuid>
#include <QTimer>
#include <QTcpSocket>

ChatServer::ChatServer(QObject *parent) : QTcpServer(parent)
{
    // 心跳检测定时器
    m_heartbeatTimer = new QTimer(this);
    connect(m_heartbeatTimer, &QTimer::timeout, this, &ChatServer::OnHeartbeat);
    m_heartbeatTimer->start(30000);

    // ACK 超时检测定时器
    m_ackTimer = new QTimer(this);
    connect(m_ackTimer, &QTimer::timeout, this, &ChatServer::OnAckTimeout);
    m_ackTimer->start(5000);
}

ChatServer::~ChatServer()
{
    m_heartbeatTimer->stop();
    m_ackTimer->stop();
    close();

    for (auto it = m_clients.begin(); it != m_clients.end(); ++it)
    {
        it.key()->abort();
        it.key()->deleteLater();
        delete it.value();
    }
    m_clients.clear();
    m_userSocket.clear();
    m_usernameToId.clear();
    m_pendingMsgs.clear();
}

void ChatServer::Start(quint16 port)
{
    if (listen(QHostAddress::Any, port))
    {
        Utils::Logger::GetLogger().Info(QString("Server listening on port: %1").arg(port));
    }
    else
    {
        Utils::Logger::GetLogger().Error(QString("Failed to start server: %1").arg(errorString()));
    }
}

// ======================== 连接管理 ========================

void ChatServer::incomingConnection(qintptr handle)
{
    QTcpSocket *socket = new QTcpSocket(this);
    if (!socket->setSocketDescriptor(handle))
    {
        Utils::Logger::GetLogger().Error(QString("SetSocketDescriptor failed: %1").arg(socket->errorString()));
        delete socket;
        return;
    }

    socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

    ClientInfo *info = new ClientInfo;
    info->socket = socket;
    info->userId = -1;
    info->lastHeartbeat = QDateTime::currentMSecsSinceEpoch();
    m_clients[socket] = info;

    connect(socket, &QTcpSocket::readyRead, this, &ChatServer::OnReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ChatServer::OnDisconnected);
    Utils::Logger::GetLogger().Info(QString("Connected: %1:%2").arg(socket->peerAddress().toString()).arg(socket->peerPort()));
}

void ChatServer::KickClient(QTcpSocket *socket)
{
    if (socket && socket->state() != QAbstractSocket::UnconnectedState)
        socket->abort();
}

// ======================== 数据收发 ========================

void ChatServer::OnReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket || !m_clients.contains(socket)) return;

    ClientInfo *info = m_clients[socket];
    info->recvBuffer.append(socket->readAll());

    // 防止撑爆内存
    if (info->recvBuffer.size() > BUFFER_LIMIT)
    {
        Utils::Logger::GetLogger().Warning("Recv buffer overflow, kicking client.");
        KickClient(socket);
        return;
    }

    quint16 type;
    QByteArray body;
    while (UnpackMessage(info->recvBuffer, type, body))
    {
        ProcessPacket(info, type, body);
        if (!m_clients.contains(socket)) return;
    }
}

void ChatServer::SendMessage(QTcpSocket *socket, quint16 type, const QByteArray &body)
{
    if (!socket || socket->state() != QAbstractSocket::ConnectedState) return;

    // 发送缓冲区背压检测
    if (socket->bytesToWrite() > SEND_BUFFER_LIMIT)
    {
        Utils::Logger::GetLogger().Warning("Send buffer overflow, kicking slow client.");
        KickClient(socket);
        return;
    }

    QByteArray packet = PackMessage(type, body);
    qint64 written = socket->write(packet);
    if (written == -1)
    {
        Utils::Logger::GetLogger().Error(QString("Write failed: %1").arg(socket->errorString()));
        KickClient(socket);
    }
}

// ======================== 消息处理 ========================

void ChatServer::ProcessPacket(ClientInfo *info, quint16 type, const QByteArray &body)
{
    info->lastHeartbeat = QDateTime::currentMSecsSinceEpoch();

    if (type == Msg_Login)
    {
        QJsonDocument doc = QJsonDocument::fromJson(body);
        if (doc.isNull()) return;
        QString username = doc.object()["username"].toString();
        QString password = doc.object()["password"].toString();
        int userId;
        QString err;

        if (Database::UserLogin(username, password, userId, err))
        {
            // 踢掉同一账号的旧连接
            if (m_userSocket.contains(userId))
            {
                QTcpSocket *oldSocket = m_userSocket[userId];
                if (oldSocket && m_clients.contains(oldSocket))
                {
                    QJsonObject kick;
                    kick["result"] = "kicked";
                    kick["error"] = "account logged in elsewhere";
                    SendMessage(oldSocket, Msg_LoginResult, QJsonDocument(kick).toJson());
                    oldSocket->flush();
                    KickClient(oldSocket);
                }
            }

            info->userId = userId;
            info->username = username;
            m_userSocket[userId] = info->socket;
            m_usernameToId[username] = userId;  // 缓存映射

            Database::UpdateUserOnline(userId, true, err);

            QJsonObject resp;
            resp["result"] = "ok";
            resp["userId"] = userId;
            resp["nickname"] = username;
            SendMessage(info->socket, Msg_LoginResult, QJsonDocument(resp).toJson());

            SendFriendList(info, err);
            SendOfflineMessages(info, err);
            BroadcastStatus(userId, true);
        }
        else
        {
            QJsonObject resp;
            resp["result"] = "fail";
            resp["error"] = err;
            SendMessage(info->socket, Msg_LoginResult, QJsonDocument(resp).toJson());
            info->socket->flush();
            info->socket->disconnectFromHost();
        }
    }
    else if (type == Msg_Logout)
    {
        if (info->userId != -1)
        {
            QString err;
            Database::UpdateUserOnline(info->userId, false, err);
            BroadcastStatus(info->userId, false);
            m_userSocket.remove(info->userId);
            m_usernameToId.remove(info->username);
            info->userId = -1;
        }
        info->socket->disconnectFromHost();
    }
    else if (type == Msg_Ping)
    {
        SendMessage(info->socket, Msg_Pong, QByteArray());
    }
    else if (type == Msg_GetFriendList)
    {
        QString err;
        SendFriendList(info, err);
    }
    else if (type == Msg_Chat)
    {
        QJsonDocument doc = QJsonDocument::fromJson(body);
        if (doc.isNull()) return;
        QString toUsername = doc.object()["to"].toString();
        QString content = doc.object()["content"].toString();
        QString err;
        ForwardChatMessage(info->userId, toUsername, content, err);
    }
    else if (type == Msg_ChatAck)
    {
        // 收到客户端的消息确认，移除待确认队列
        QJsonDocument doc = QJsonDocument::fromJson(body);
        if (doc.isNull()) return;
        QString msgId = doc.object()["msgId"].toString();
        if (m_pendingMsgs.contains(msgId))
        {
            PendingMsg pm = m_pendingMsgs.take(msgId);
            // 通知发送方：已送达
            NotifySender(pm.fromId, pm.msgId, "delivered");
        }
    }
    else if (type == Msg_AddFriend)
    {
        QJsonDocument doc = QJsonDocument::fromJson(body);
        if (doc.isNull()) return;
        int userId = doc.object()["userId"].toInt();
        int friendId = doc.object()["friendId"].toInt();
        QString err;
        Database::AddFriend(userId, friendId, err);
    }
    else if (type == Msg_Register)
    {
        QJsonDocument doc = QJsonDocument::fromJson(body);
        if (doc.isNull()) return;
        QString username = doc.object()["username"].toString();
        QString password = doc.object()["password"].toString();
        QString nickname = doc.object()["nickname"].toString();
        int sex = doc.object()["sex"].toInt();
        QString birth = doc.object()["birth"].toString();
        QString signature = doc.object()["signature"].toString();
        QString err;
        QJsonObject resp;
        if (Database::UserRegister(username, password, nickname, sex, birth, signature, err))
            resp["result"] = "ok";
        else
        {
            resp["result"] = "fail";
            resp["error"] = err;
        }
        SendMessage(info->socket, Msg_Register, QJsonDocument(resp).toJson());
        info->socket->flush();
        info->socket->disconnectFromHost();
    }
}

// ======================== 消息转发（核心） ========================

int ChatServer::ResolveUserId(const QString &username, QString &err)
{
    // 优先查内存缓存
    if (m_usernameToId.contains(username))
        return m_usernameToId[username];
    // 缓存未命中才查库（离线用户）
    int id = Database::GetUserIdByUsername(username, err);
    if (id != -1)
        m_usernameToId[username] = id;

    return id;
}

void ChatServer::ForwardChatMessage(int fromId, const QString &toUsername,
                                    const QString &content, QString &err)
{
    int toId = ResolveUserId(toUsername, err);
    if (toId == -1) return;

    Utils::Logger::GetLogger().Info(QString("ID %1 Send Msg[%2] to %3").arg(fromId).arg(content).arg(toId));
    QString msgId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    qint64 timestamp = QDateTime::currentMSecsSinceEpoch();

    QJsonObject msg;
    msg["msgId"] = msgId;
    msg["fromUserId"] = fromId;
    msg["content"] = content;
    msg["timestamp"] = timestamp;
    QByteArray body = QJsonDocument(msg).toJson();

    if (m_userSocket.contains(toId))
    {
        QTcpSocket *toSocket = m_userSocket[toId];
        if (toSocket && toSocket->state() == QAbstractSocket::ConnectedState)
        {
            SendMessage(toSocket, Msg_Chat, body);

            // 放入待确认队列
            PendingMsg pm;
            pm.fromId = fromId;
            pm.toId = toId;
            pm.content = content;
            pm.timestamp = timestamp;
            pm.msgId = msgId;
            pm.retryCount = 0;
            m_pendingMsgs[msgId] = pm;

            // 通知发送方：已发送
            NotifySender(fromId, msgId, "sent");
        }
        else
        {
            // socket 失效，清理并存离线
            m_userSocket.remove(toId);
            Database::StoreOfflineMsg(fromId, toId, content, err);
            NotifySender(fromId, msgId, "stored_offline");
        }
    }
    else
    {
        Database::StoreOfflineMsg(fromId, toId, content, err);
        NotifySender(fromId, msgId, "stored_offline");
    }
}

void ChatServer::NotifySender(int fromId, const QString &msgId, const QString &status)
{
    if (!m_userSocket.contains(fromId)) return;
    QJsonObject ack;
    ack["msgId"] = msgId;
    ack["status"] = status;  // "sent" / "delivered" / "stored_offline"
    SendMessage(m_userSocket[fromId], Msg_ChatAck, QJsonDocument(ack).toJson());
}

// ======================== ACK 超时处理 ========================

void ChatServer::OnAckTimeout()
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    QStringList expiredIds;

    for (auto it = m_pendingMsgs.begin(); it != m_pendingMsgs.end(); ++it)
    {
        PendingMsg &pm = it.value();
        if (now - pm.timestamp > ACK_TIMEOUT)
        {
            if (pm.retryCount < ACK_MAX_RETRY)
            {
                // 重试发送
                pm.retryCount++;
                if (m_userSocket.contains(pm.toId))
                {
                    QJsonObject msg;
                    msg["msgId"] = pm.msgId;
                    msg["fromUserId"] = pm.fromId;
                    msg["content"] = pm.content;
                    msg["timestamp"] = pm.timestamp;
                    SendMessage(m_userSocket[pm.toId], Msg_Chat,
                                QJsonDocument(msg).toJson());
                    Utils::Logger::GetLogger().Info(QString("Retry msg: %1 attempt: %2").arg(pm.msgId).arg(pm.retryCount));
                }
                else
                {
                    expiredIds.append(it.key());
                }
            }
            else
            {
                // 超过重试次数，存离线消息
                expiredIds.append(it.key());
            }
        }
    }

    // 处理过期消息
    for (const QString &id : expiredIds)
    {
        PendingMsg pm = m_pendingMsgs.take(id);
        QString err;
        Database::StoreOfflineMsg(pm.fromId, pm.toId, pm.content, err);
        NotifySender(pm.fromId, pm.msgId, "stored_offline");
        Utils::Logger::GetLogger().Info(QString("Msg: %1 ACK timeout, stored offline").arg(pm.msgId));
    }
}

// ======================== 好友与离线消息 ========================

void ChatServer::BroadcastStatus(int userId, bool online)
{
    QJsonObject obj;
    obj["userId"] = userId;
    obj["online"] = online;
    QByteArray body = QJsonDocument(obj).toJson();
    QByteArray packet = PackMessage(Msg_StatusUpdate, body);

    for (auto *cli : m_clients)
    {
        if (cli->userId != userId && cli->userId != -1
            && cli->socket->state() == QAbstractSocket::ConnectedState)
        {
            cli->socket->write(packet);
        }
    }
}

void ChatServer::SendFriendList(ClientInfo *info, QString &err)
{
    QList<UserInfo> friends = Database::GetFriendList(info->userId, err);
    QJsonArray arr;
    for (const auto &f : friends)
    {
        QJsonObject obj;
        obj["userId"] = f.id;
        obj["username"] = f.username;
        obj["nickname"] = f.nickname;
        obj["online"] = f.online;
        arr.append(obj);
    }
    QJsonObject root;
    root["friends"] = arr;
    SendMessage(info->socket, Msg_FriendList, QJsonDocument(root).toJson());
}

void ChatServer::SendOfflineMessages(ClientInfo *info, QString &err)
{
    auto msgs = Database::GetOfflineMsgs(info->userId, err);
    for (const auto &msg : msgs)
    {
        int fromId = msg[0].toInt();
        QString content = msg[1];
        QString timestamp = msg.size() > 2 ? msg[2] : "0";  // 带上时间戳

        QJsonObject obj;
        obj["msgId"] = QUuid::createUuid().toString(QUuid::WithoutBraces);
        obj["fromUserId"] = fromId;
        obj["content"] = content;
        obj["timestamp"] = timestamp.toLongLong();
        SendMessage(info->socket, Msg_Chat, QJsonDocument(obj).toJson());
    }
    Database::ClearOfflineMsgs(info->userId, err);
}

// ======================== 断线与心跳 ========================

void ChatServer::OnDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket || !m_clients.contains(socket)) return;

    ClientInfo *info = m_clients.take(socket);
    Utils::Logger::GetLogger().Info(QString("Disconnected: %1:%2").arg(socket->peerAddress().toString()).arg(socket->peerPort()));

    if (info->userId != -1)
    {
        // 将该用户所有未确认消息转存为离线消息
        QStringList toRemove;
        for (auto it = m_pendingMsgs.begin(); it != m_pendingMsgs.end(); ++it)
        {
            if (it.value().toId == info->userId)
            {
                QString err;
                Database::StoreOfflineMsg(it.value().fromId, it.value().toId,
                                          it.value().content, err);
                NotifySender(it.value().fromId, it.value().msgId, "stored_offline");
                toRemove.append(it.key());
            }
        }
        for (const QString &id : toRemove)
            m_pendingMsgs.remove(id);

        QString err;
        Database::UpdateUserOnline(info->userId, false, err);
        BroadcastStatus(info->userId, false);
        m_userSocket.remove(info->userId);
        m_usernameToId.remove(info->username);
    }
    delete info;
    socket->deleteLater();
}

void ChatServer::OnHeartbeat()
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    QList<QTcpSocket*> timeoutList;

    for (auto &info : m_clients)
    {
        if (info->userId != -1 && now - info->lastHeartbeat > HEARTBEAT_TIMEOUT)
        {
            timeoutList.append(info->socket);
        }
        // else if (info->userId == -1 && now - info->lastHeartbeat > LOGIN_TIMEOUT)
        // {
        //     timeoutList.append(info->socket);
        // }
    }

    for (auto *sock : timeoutList)
    {
        Utils::Logger::GetLogger().Info("Heartbeat timeout, kicking");
        sock->abort();
    }
}