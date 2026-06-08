#include "../stdafx.h"
#include "networkmanager.h"
#include "../protocol.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QDebug>
#include <QThread>

namespace Network
{
NetworkManager* g_pNetworkMgr = nullptr;

NetworkManager* GetInstance(QTcpSocket *parent)
{
    if (!g_pNetworkMgr)
    {
        g_pNetworkMgr = new NetworkManager;
    }

    return g_pNetworkMgr;
}

NetworkManager::NetworkManager(QTcpSocket *parent)
    : QTcpSocket(parent)
    , m_funcConnCallback(nullptr)
    , m_pConnTimer(nullptr)
    , m_pPingTimer(nullptr)
{
    qDebug () << "NetworkManager"<< QThread::currentThreadId();
}

NetworkManager::~NetworkManager()
{
    if (m_pPingTimer)
    {
        m_pPingTimer->stop();
        m_pPingTimer->deleteLater();
        m_pPingTimer = nullptr;
    }

    if (m_pConnTimer)
    {
        m_pConnTimer->stop();
        m_pConnTimer->deleteLater();
        m_pConnTimer = nullptr;
    }
}

void NetworkManager::ConnectToServer(const QString &host, quint16 port, std::function<void (bool)> callback)
{
    qDebug () << "ConnectToServer"<< QThread::currentThreadId();
    m_funcConnCallback = callback;
    emit UpdateConnState(QTcpSocket::ConnectingState);
    connectToHost(host, port);
}

void NetworkManager::SendPacket(quint16 type, const QByteArray &body)
{
    if (state() == QAbstractSocket::ConnectedState)
    {
        write(PackMessage(type, body));
    }
}

void NetworkManager::SendLogin(const QString &username, const QString &password)
{
    QJsonObject obj;
    obj["username"] = username;
    obj["password"] = password;
    SendPacket(Msg_Login, QJsonDocument(obj).toJson());
}

void NetworkManager::SendChat(const QString &toUsername, const QString &content)
{
    QJsonObject obj;
    obj["to"] = toUsername;
    obj["content"] = content;
    SendPacket(Msg_Chat, QJsonDocument(obj).toJson());
}

void NetworkManager::AddFriend(const QString& userName, const QString& friendName)
{
    QJsonObject obj;
    obj["userName"] = userName;
    obj["friendName"] = friendName;
    SendPacket(Msg_AddFriend, QJsonDocument(obj).toJson());
}

void NetworkManager::SendPing()
{
    SendPacket(Msg_Ping, QByteArray());
}

void NetworkManager::SendGetFriendList()
{
    SendPacket(Msg_GetFriendList, QByteArray());
}

void NetworkManager::SendRegisterUser(const QString &username, const QString &password, const QString &nickname, int sex, const QString &birth, const QString &signature)
{
    QJsonObject obj;
    obj["username"] = username;
    obj["password"] = password;
    obj["nickname"] = nickname;
    obj["sex"] = sex;
    obj["birth"] = birth;
    obj["signature"] = signature;
    SendPacket(Msg_Register, QJsonDocument(obj).toJson());
}

void NetworkManager::onStartConnTimer()
{
    if (m_pConnTimer && !m_pConnTimer->isActive())
        m_pConnTimer->start();
}

void NetworkManager::onStopConnTimer()
{
    if (m_pConnTimer && m_pConnTimer->isActive())
        m_pConnTimer->stop();
}

void NetworkManager::DoWork()
{
    emit UpdateConnState(state());
    if (state() == QTcpSocket::SocketState::UnconnectedState)
    {
        ConnectToServer(SERVER_IP, LINSTEN_PORT);
    }
}

bool NetworkManager::IsOnline()
{
    return (state() == QAbstractSocket::ConnectedState);
}

void NetworkManager::InitNetwork()
{
    // NOTE:
    connect(this, &QTcpSocket::connected, this, &NetworkManager::OnSocketConnected);
    connect(this, &QTcpSocket::disconnected, this, &NetworkManager::OnSocketDisconnected);
    connect(this, &QTcpSocket::readyRead, this, &NetworkManager::OnReadyRead);
    connect(this, &QTcpSocket::errorOccurred, this, &NetworkManager::OnErrorOccurred);

    // NOTE: Request Connection
    DoWork();
    m_pConnTimer = new QTimer(this);
    connect(m_pConnTimer, &QTimer::timeout, this, &NetworkManager::DoWork);
    m_pConnTimer->start(3000);

    // NOTE: Enable Heartbeat Timer
    m_pPingTimer = new QTimer(this);
    connect(m_pPingTimer, &QTimer::timeout, this, &NetworkManager::SendPing);
    m_pPingTimer->start(30000);
}

void NetworkManager::OnReadyRead()
{
    m_recvBuffer.append(readAll());
    quint16 type;
    QByteArray body;
    while (UnpackMessage(m_recvBuffer, type, body))
    {
        switch (type)
        {
        case Msg_LoginResult: {
            QJsonDocument doc = QJsonDocument::fromJson(body);
            bool ok = doc.object()["result"].toString() == "ok";
            int userId = doc.object()["userId"].toInt();
            QString err = doc.object()["error"].toString();
            emit LoginResult(ok, userId, err);
            break;
        }
        case Msg_FriendList: {
            QJsonDocument doc = QJsonDocument::fromJson(body);
            emit FriendListReceived(doc.object()["friends"].toArray());
            break;
        }
        case Msg_Chat: {
            QJsonDocument doc = QJsonDocument::fromJson(body);
            int fromUserId = doc.object()["fromUserId"].toInt();
            QString content = doc.object()["content"].toString();
            qint64 ts = doc.object()["timestamp"].toVariant().toLongLong();
            emit ChatMessageReceived(fromUserId, content, ts);
            break;
        }
        case Msg_ChatAck: {
            QJsonDocument doc = QJsonDocument::fromJson(body);
            QString msgId = doc.object()["msgId"].toString();
            QString status = doc.object()["status"].toString();
            emit ChatMsgResult(msgId, status);
            break;
        }
        case Msg_StatusUpdate: {
            QJsonDocument doc = QJsonDocument::fromJson(body);
            int userId = doc.object()["userId"].toInt();
            bool online = doc.object()["online"].toBool();
            emit StatusUpdateReceived(userId, online);
            break;
        }
        case Msg_RegisterResult: {
            QJsonDocument doc = QJsonDocument::fromJson(body);
            bool ok = doc.object()["result"].toString() == "ok";
            QString err = doc.object()["error"].toString();
            emit RegisterResult(ok, err);
            break;
        }
        case Msg_AddFriendResult:{
            QJsonDocument doc = QJsonDocument::fromJson(body);
            bool ok = doc.object()["result"].toString() == "ok";
            QString err = doc.object()["error"].toString();
            emit AddFriendResult(ok, err);
            break;
        }
        case Msg_Pong:
            // 心跳响应，无需处理
            break;
        default:
            qDebug() << "Unknown message type" << type;
        }
    }
}

void NetworkManager::OnSocketConnected()
{
    if (m_funcConnCallback)
    {
        m_funcConnCallback(true);
        m_funcConnCallback = nullptr;
    }
}

void NetworkManager::OnSocketDisconnected()
{

}

void NetworkManager::OnErrorOccurred(QAbstractSocket::SocketError error)
{
    emit ErrorOccurred(errorString());
    if (m_funcConnCallback)
    {
        m_funcConnCallback(false);
        m_funcConnCallback = nullptr;
    }
}

}
