#include "util.h"

#include <QStringList>
#include <QCryptographicHash>
#include <QByteArray>

QStringList Util::SplitStr(const QString &s, const QString &delimiter, Qt::CaseSensitivity cs)
{
    QStringList sLst;
    if (s.isEmpty() || delimiter.isEmpty())
        return sLst;

    QString sPart;
    int iFindPos = -1, iStartPos = 0, iDelimiterSize = delimiter.size();
    while ((iFindPos = s.indexOf(delimiter, iStartPos, cs)) != -1)
    {
        sPart = s.mid(iStartPos, iFindPos - iStartPos);
        if (!sPart.isEmpty()) sLst.push_back(sPart); // 空串不入链表
        iStartPos = iFindPos + iDelimiterSize;
    }
    if (iStartPos < s.size())
        sLst.push_back(s.mid(iStartPos, s.size() - iStartPos));

    return sLst;
}

QString Util::TrimmedStr(const QString &str)
{
    return str.trimmed();
}

QString Util::ReplaceStr(const QString &s, const QString& oldStr, const QString & newStr, bool bReplaceOne, Qt::CaseSensitivity cs)
{
    QString sRst = s;
    if (s.isEmpty() || oldStr.isEmpty() || newStr.isEmpty())
        return sRst;

    int iFindPos = -1;
    if (bReplaceOne)
    {
        if (-1 != (iFindPos = sRst.indexOf(oldStr, 0, cs)))
        {
            sRst.replace(iFindPos, oldStr.size(), newStr);
        }
    }
    else
    {
        sRst.replace(oldStr, newStr, cs);
    }

    return sRst;
}


//////////////////// 加解密
// 加密：明文 + 密钥 → Base64 密文
QString Util::encrypt(const QString &plain, const QString &key)
{
    QByteArray data = plain.toUtf8();
    // 将密钥固定为 16 字节（MD5）
    QByteArray keyData = QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Md5);
    // XOR 加密
    for (int i = 0; i < data.size(); ++i)
        data[i] = data[i] ^ keyData[i % 16];
    return data.toBase64();
}

// 解密：Base64 密文 + 密钥 → 明文
QString Util::decrypt(const QString &cipherBase64, const QString &key)
{
    QByteArray data = QByteArray::fromBase64(cipherBase64.toUtf8());
    QByteArray keyData = QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Md5);
    for (int i = 0; i < data.size(); ++i)
        data[i] = data[i] ^ keyData[i % 16];
    return QString::fromUtf8(data);
}
