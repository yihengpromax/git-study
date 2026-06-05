#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QHash>

struct ClientInfo
{
    QTcpSocket *socket;
    int userId;
    QString username;
    QByteArray recvBuffer;   // 用于粘包处理
    qint64 lastHeartbeat;
};

// 待确认消息
struct PendingMsg
{
    int fromId;
    int toId;
    QString content;
    qint64 timestamp;
    QString msgId;
    int retryCount = 0;
};

class QTcpSocket;
class QTimer;
class ChatServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit ChatServer(QObject *parent = nullptr);
    ~ChatServer();
    void Start(quint16 port);

protected:
    void incomingConnection(qintptr handle) override;

private slots:
    void OnReadyRead();
    void OnDisconnected();
    void OnHeartbeat();  // 定时检查心跳
    void OnAckTimeout();

private:
    void ProcessPacket(ClientInfo *info, quint16 type, const QByteArray &body);
    void SendMessage(QTcpSocket *socket, quint16 type, const QByteArray &body);
    void BroadcastStatus(int userId, bool online);
    void SendFriendList(ClientInfo *info, QString& err);
    void ForwardChatMessage(int fromId, const QString &toUsername, const QString &content, QString& err);
    void SendOfflineMessages(ClientInfo *info, QString& err);

    // 工具方法
    void KickClient(QTcpSocket *socket);
    void NotifySender(int fromId, const QString &msgId, const QString &status);
    int ResolveUserId(const QString &username, QString &err);

private:
    QHash<QTcpSocket*, ClientInfo*> m_clients;
    QHash<int, QTcpSocket*> m_userSocket; // userId -> socket
    QHash<QString, int> m_usernameToId;            // username → userId（内存缓存）
    QHash<QString, PendingMsg> m_pendingMsgs;      // msgId → 待确认消息
    QTimer *m_heartbeatTimer = nullptr;
    QTimer *m_ackTimer = nullptr;
};

#endif // SERVER_H