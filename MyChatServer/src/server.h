#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QHash>

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

private:
    struct ClientInfo
    {
        QTcpSocket *socket;
        int userId;
        QString username;
        QByteArray recvBuffer;   // 用于粘包处理
        qint64 lastHeartbeat;
    };
    QHash<QTcpSocket*, ClientInfo*> m_clients;
    QHash<int, QTcpSocket*> m_userSocket; // userId -> socket
    QTimer *m_heartbeatTimer;

private:
    void ProcessPacket(ClientInfo *info, quint16 type, const QByteArray &body);
    void SendMessage(QTcpSocket *socket, quint16 type, const QByteArray &body);
    void BroadcastStatus(int userId, bool online);
    void SendFriendList(ClientInfo *info, QString& err);
    void ForwardChatMessage(int fromId, const QString &toUsername, const QString &content, QString& err);
    void SendOfflineMessages(ClientInfo *info, QString& err);
};

#endif // SERVER_H