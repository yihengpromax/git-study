#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    static NetworkManager* GetInstance(QObject *parent = nullptr);
    void ConnectToServer(const QString &host, quint16 port);
    void SendLogin(const QString &username, const QString &password);
    void SendChat(const QString &toUsername, const QString &content);
    void AddFriend(int userId, int friendId);
    void SendPing();
    void SendGetFriendList();
    bool IsOnline();

signals:
    void Connected();
    void Disconnected();
    void LoginResult(bool ok, int userId);
    void FriendListReceived(const QJsonArray &friends);
    void ChatMessageReceived(int fromUserId, const QString &content, qint64 timestamp);
    void StatusUpdateReceived(int userId, bool online);
    void ErrorOccurred(const QString &err);

private slots:
    void OnReadyRead();
    void OnSocketConnected();
    void OnSocketDisconnected();

private:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();
    void SendPacket(quint16 type, const QByteArray &body);

private:
    static NetworkManager *m_instance;
    QTcpSocket *m_socket;
    QByteArray m_recvBuffer;
    bool m_bOnline;
};

#endif // NETWORKMANAGER_H
