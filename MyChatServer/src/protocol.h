#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QDataStream>
#include <QMetaType>
#include <QIODevice>

// 消息类型枚举
enum MessageType {
    Msg_Login          = 0x01,
    Msg_LoginResult    = 0x02,
    Msg_Logout         = 0x03,
    Msg_Ping           = 0x04,
    Msg_Pong           = 0x05,
    Msg_Register       = 0x06,
    Msg_Chat           = 0x10,
    Msg_ChatAck        = 0x11,
    Msg_GetFriendList  = 0x20,
    Msg_FriendList     = 0x21,
    Msg_StatusUpdate   = 0x22,
    Msg_AddFriend      = 0x23,
    Msg_OfflineMsg     = 0x30
};

#define LINSTEN_PORT 12580

static const int BUFFER_LIMIT = 1024 * 1024;          // 1MB 接收缓冲上限
static const int SEND_BUFFER_LIMIT = 512 * 1024;      // 512KB 发送缓冲上限
static const int HEARTBEAT_TIMEOUT = 60000;            // 60秒心跳超时
static const int LOGIN_TIMEOUT = 15000;                // 15秒登录超时
static const int ACK_TIMEOUT = 10000;                  // 10秒 ACK 超时
static const int ACK_MAX_RETRY = 2;                    // ACK 最大重试次数

// 协议包结构：长度(quint32) + 类型(quint16) + 体(QByteArray)
struct MessagePacket
{
    quint32 length;     // 包括 length 字段本身？建议：不包括length字段自身，只包括type+body的长度
    quint16 type;
    QByteArray body;
};

// 辅助函数：打包
inline QByteArray PackMessage(quint16 type, const QByteArray &body)
{
    QByteArray packet;
    QDataStream stream(&packet, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_15);
    quint32 len = sizeof(quint16) + body.size();  // type(2) + body
    stream << len << type;
    if (!body.isEmpty())
        stream.writeRawData(body.data(), body.size());

    return packet;
}

// 解包：从缓冲区解析一条完整消息，返回是否成功
inline bool UnpackMessage(QByteArray &buffer, quint16 &type, QByteArray &body)
{
    if (buffer.size() < (int)sizeof(quint32)) return false;
    quint32 len;
    QDataStream stream(buffer);
    stream.setVersion(QDataStream::Qt_5_15);
    stream >> len;
    if (buffer.size() < (int)(sizeof(quint32) + len)) return false;
    stream >> type;
    body.resize(len - sizeof(quint16));
    if (!body.isEmpty())
        stream.readRawData(body.data(), body.size());
    buffer.remove(0, sizeof(quint32) + len);

    return true;
}

#endif // PROTOCOL_H