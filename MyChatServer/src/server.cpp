#include "stdafx.h"
#include "server.h"
#include "database.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>
#include <QDateTime>
#include <QDebug>
#include <QJsonArray>
#include <QTcpSocket>
#include <QTimer>

ChatServer::ChatServer(QObject *parent) : QTcpServer(parent)
{
    m_heartbeatTimer = new QTimer(this);
    connect(m_heartbeatTimer, &QTimer::timeout, this, &ChatServer::OnHeartbeat);
    m_heartbeatTimer->start(30000); // 30秒心跳检查
}

ChatServer::~ChatServer()
{
    if (m_clients.size() > 0)
    {
        m_clients.clear();
    }

    if (m_userSocket.size() > 0)
    {
        m_userSocket.clear();
    }
}

void ChatServer::Start(quint16 port)
{
    if (listen(QHostAddress::Any, port))
        qDebug() << "====================================== Server listening on port" << port;
    else
        qDebug() << "====================================== Failed to start server";
}

void ChatServer::incomingConnection(qintptr handle)
{
    QTcpSocket *socket = new QTcpSocket(this);
    socket->setSocketDescriptor(handle);
    ClientInfo *info = new ClientInfo;
    info->socket = socket;
    info->userId = -1;
    info->lastHeartbeat = QDateTime::currentMSecsSinceEpoch();
    m_clients[socket] = info;
    connect(socket, &QTcpSocket::readyRead, this, &ChatServer::OnReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ChatServer::OnDisconnected);
    qDebug() << "====================================== Connected.";
}

void ChatServer::OnReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket || !m_clients.contains(socket)) return;
    ClientInfo *info = m_clients[socket];
    info->recvBuffer.append(socket->readAll());

    quint16 type;
    QByteArray body;
    while (UnpackMessage(info->recvBuffer, type, body))
    {
        ProcessPacket(info, type, body);
    }
}

void ChatServer::ProcessPacket(ClientInfo *info, quint16 type, const QByteArray &body)
{
    // 更新心跳时间
    info->lastHeartbeat = QDateTime::currentMSecsSinceEpoch();

    if (type == Msg_Login)
    {
        QJsonDocument doc = QJsonDocument::fromJson(body);
        if (doc.isNull()) return;
        QString username = doc.object()["username"].toString();
        QString password = doc.object()["password"].toString();
        int userId;
        if (Database::UserLogin(username, password, userId)) {
            info->userId = userId;
            info->username = username;
            m_userSocket[userId] = info->socket;
            // 发送登录成功
            QJsonObject resp;
            resp["result"] = "ok";
            resp["userId"] = userId;
            resp["nickname"] = username; // 可从DB取昵称
            SendMessage(info->socket, Msg_LoginResult, QJsonDocument(resp).toJson());

            // 发送好友列表
            SendFriendList(info);

            // 发送离线消息
            SendOfflineMessages(info);

            // 广播在线状态
            BroadcastStatus(userId, true);
        }
        else {
            QJsonObject resp;
            resp["result"] = "fail";
            SendMessage(info->socket, Msg_LoginResult, QJsonDocument(resp).toJson());
            info->socket->disconnectFromHost();
        }
    }
    else if (type == Msg_Logout) {
        if (info->userId != -1)
        {
            Database::UpdateUserOnline(info->userId, false);
            BroadcastStatus(info->userId, false);
            m_userSocket.remove(info->userId);
        }
        info->socket->disconnectFromHost();
    }
    else if (type == Msg_Ping) {
        SendMessage(info->socket, Msg_Pong, QByteArray());
    }
    else if (type == Msg_GetFriendList) {
        SendFriendList(info);
    }
    else if (type == Msg_Chat) {
        QJsonDocument doc = QJsonDocument::fromJson(body);
        if (doc.isNull()) return;
        QString toUsername = doc.object()["to"].toString();
        QString content = doc.object()["content"].toString();
        ForwardChatMessage(info->userId, toUsername, content);
    }
    else if(type == Msg_AddFriend){
        QJsonDocument doc = QJsonDocument::fromJson(body);
        if (doc.isNull()) return;
        int userId = doc.object()["userId"].toInt();
        int friendId = doc.object()["friendId"].toInt();
        Database::AddFriend(userId, friendId);
    }
    else if (type == Msg_ChatAck) {
        // 可选：消息确认，本示例忽略
    }
}

void ChatServer::SendMessage(QTcpSocket *socket, quint16 type, const QByteArray &body)
{
    QByteArray packet = PackMessage(type, body);
    socket->write(packet);
}

void ChatServer::BroadcastStatus(int userId, bool online)
{
    QJsonObject obj;
    obj["userId"] = userId;
    obj["online"] = online;
    QByteArray body = QJsonDocument(obj).toJson();
    QByteArray packet = PackMessage(Msg_StatusUpdate, body);
    // 向该用户的所有好友广播（简单实现：遍历在线用户，只发送给好友）
    // 实际应查好友表，这里为简化，广播给所有在线客户端（仅演示）
    for (auto *cli : m_clients) {
        if (cli->userId != userId && cli->userId != -1) {
            cli->socket->write(packet);
        }
    }
}

void ChatServer::SendFriendList(ClientInfo *info)
{
    QList<UserInfo> friends = Database::GetFriendList(info->userId);
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

void ChatServer::ForwardChatMessage(int fromId, const QString &toUsername, const QString &content)
{
    int toId = Database::GetUserIdByUsername(toUsername);
    if (toId == -1) return;
    QJsonObject msg;
    msg["fromUserId"] = fromId;
    msg["content"] = content;
    msg["timestamp"] = QDateTime::currentMSecsSinceEpoch();
    QByteArray body = QJsonDocument(msg).toJson();
    if (m_userSocket.contains(toId)) {
        QTcpSocket *toSocket = m_userSocket[toId];
        SendMessage(toSocket, Msg_Chat, body);
    } else {
        Database::StoreOfflineMsg(fromId, toId, content);
    }
}

void ChatServer::SendOfflineMessages(ClientInfo *info)
{
    auto msgs = Database::GetOfflineMsgs(info->userId);
    for (const auto &msg : msgs) {
        int fromId = msg[0].toInt();
        QString content = msg[1];
        // 重新构造Chat消息发给客户端
        QJsonObject obj;
        obj["fromUserId"] = fromId;
        obj["content"] = content;
        obj["timestamp"] = 0;
        SendMessage(info->socket, Msg_Chat, QJsonDocument(obj).toJson());
    }
    Database::ClearOfflineMsgs(info->userId);
}

void ChatServer::OnDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket || !m_clients.contains(socket)) return;
    ClientInfo *info = m_clients[socket];
    if (info->userId != -1) {
        Database::UpdateUserOnline(info->userId, false);
        BroadcastStatus(info->userId, false);
        m_userSocket.remove(info->userId);
    }
    delete info;
    m_clients.remove(socket);
    socket->deleteLater();
    qDebug() << "Disconnected";
}

void ChatServer::OnHeartbeat()
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    QList<QTcpSocket*> timeoutList;
    for (auto *info : m_clients) {
        if (now - info->lastHeartbeat > 60000) { // 60秒无心跳则断开
            timeoutList.append(info->socket);
        }
    }
    for (auto *sock : timeoutList) {
        sock->disconnectFromHost();
    }
}