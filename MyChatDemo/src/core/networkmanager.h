#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QTcpSocket>

namespace Network
{
class NetworkManager : public QTcpSocket
{
    Q_OBJECT

public:
    explicit NetworkManager(QTcpSocket *parent = nullptr);
    ~NetworkManager();
    void SendPacket(quint16 type, const QByteArray &body);
    void ConnectToServer(const QString &host, quint16 port, std::function<void (bool)> callback = nullptr);
    bool IsOnline();

public slots:
    void InitNetwork();
    void SendLogin(const QString &username, const QString &password);
    void SendChat(const QString &toUsername, const QString &content);
    void AddFriend(const QString& userName, const QString& friendName);
    void SendPing();
    void SendGetFriendList();
    void SendRegisterUser(const QString &username, const QString &password, const QString &nickname, int sex, const QString &birth, const QString &signature);
    void SendGetofflineMsgReq(const QString &username);
    void DoWork();
    void onStartConnTimer();
    void onStopConnTimer();

signals:
    void UpdateConnState(int state);
    void LoginResult(bool ok, int userId, const QString &err);
    void FriendListReceived(const QJsonArray &friends);
    void ChatMessageReceived(int fromId, const QString & fromUserName, const QString &content, qint64 ts);
    void ChatMsgResult(const QString& msgId, const QString& msg);
    void StatusUpdateReceived(int userId, bool online);
    void ErrorOccurred(const QString &err);
    void RegisterResult(bool ok, const QString &err);
    void AddFriendResult(bool ok, const QString &err);

private slots:
    void OnReadyRead();
    void OnSocketConnected();
    void OnSocketDisconnected();
    void OnErrorOccurred(QAbstractSocket::SocketError error);

private:
    QByteArray m_recvBuffer;
    std::function<void (bool)> m_funcConnCallback;
    QTimer *m_pConnTimer, *m_pPingTimer;
    int m_iWaitCnt;
};

NetworkManager* GetInstance(QTcpSocket *parent = nullptr);

} // namespace Network


#endif // NETWORKMANAGER_H
