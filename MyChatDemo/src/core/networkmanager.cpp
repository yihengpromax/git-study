#include "networkmanager.h"
#include "../protocol.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QDebug>
#include <QThread>

NetworkManager* NetworkManager::m_instance = nullptr;

NetworkManager* NetworkManager::GetInstance(QObject *parent)
{
    if (!m_instance)
        m_instance = new NetworkManager(parent);

    return m_instance;
}

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
    , m_funcConnCallback(nullptr)
{
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::connected, this, &NetworkManager::OnSocketConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &NetworkManager::OnSocketDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &NetworkManager::OnReadyRead);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &NetworkManager::OnErrorOccurred);

    // 启动心跳定时器
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &NetworkManager::SendPing);
    timer->start(30000);
}

NetworkManager::~NetworkManager()
{

}

void NetworkManager::ConnectToServer(const QString &host, quint16 port, std::function<void (bool)> callback)
{
    m_funcConnCallback = callback;
    m_socket->connectToHost(host, port);
}

void NetworkManager::SendPacket(quint16 type, const QByteArray &body)
{
    if (m_socket->state() == QAbstractSocket::ConnectedState)
    {
        m_socket->write(PackMessage(type, body));
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

void NetworkManager::AddFriend(int userId, int friendId)
{
    QJsonObject obj;
    obj["userId"] = userId;
    obj["friendId"] = friendId;
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

bool NetworkManager::IsOnline()
{
    return (m_socket->state() == QAbstractSocket::ConnectedState);
}

void NetworkManager::OnReadyRead()
{
    m_recvBuffer.append(m_socket->readAll());
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
        case Msg_StatusUpdate: {
            QJsonDocument doc = QJsonDocument::fromJson(body);
            int userId = doc.object()["userId"].toInt();
            bool online = doc.object()["online"].toBool();
            emit StatusUpdateReceived(userId, online);
            break;
        }
        case Msg_Register: {
            QJsonDocument doc = QJsonDocument::fromJson(body);
            bool ok = doc.object()["result"].toString() == "ok";
            QString err = doc.object()["error"].toString();
            emit RegisterResult(ok, err);
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
    emit Connected();
    if (m_funcConnCallback)
    {
        m_funcConnCallback(true);
        m_funcConnCallback = nullptr;
    }
}

void NetworkManager::OnSocketDisconnected()
{
    emit Disconnected();
}

void NetworkManager::OnErrorOccurred(QAbstractSocket::SocketError error)
{
    emit ErrorOccurred(m_socket->errorString());
    if (m_funcConnCallback)
    {
        m_funcConnCallback(false);
        m_funcConnCallback = nullptr;
    }
}
