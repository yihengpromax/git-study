#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    static NetworkManager* GetInstance(QObject *parent = nullptr);

public:
    void ConnectToServer(const QString &host, quint16 port, std::function<void (bool)> callback = nullptr);
    void SendLogin(const QString &username, const QString &password);
    void SendChat(const QString &toUsername, const QString &content);
    void AddFriend(int userId, int friendId);
    void SendPing();
    void SendGetFriendList();
    void SendRegisterUser(const QString &username, const QString &password, const QString &nickname, int sex, const QString &birth, const QString &signature);
    bool IsOnline();

signals:
    void Connected();
    void Disconnected();
    void LoginResult(bool ok, int userId, const QString &err);
    void FriendListReceived(const QJsonArray &friends);
    void ChatMessageReceived(int fromUserId, const QString &content, qint64 timestamp);
    void StatusUpdateReceived(int userId, bool online);
    void ErrorOccurred(const QString &err);
    void RegisterResult(bool ok, const QString &err);

private slots:
    void OnReadyRead();
    void OnSocketConnected();
    void OnSocketDisconnected();
    void OnErrorOccurred(QAbstractSocket::SocketError error);

private:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();
    void SendPacket(quint16 type, const QByteArray &body);

private:
    static NetworkManager *m_instance;
    QTcpSocket *m_socket;
    QByteArray m_recvBuffer;
    std::function<void (bool)> m_funcConnCallback;
};

#endif // NETWORKMANAGER_H
