#pragma once

#include <QString>

namespace Util
{
    ///////////// 字符串处理
    /***************************************************************
    @author: yiheng
    @date: 2026/6/1
    @description: 根据sep分割字符串
    @see:
    @param: s待分割的源字符串
    @param: delimiter分割符，支持多字符分割符
    @param: cs, 是否区分大小写
    @return: 返回分割后的字符串链表
    @fix:
    @update:
    ***************************************************************/
    QStringList SplitStr(const QString &s, const QString &delimiter, Qt::CaseSensitivity cs = Qt::CaseSensitive);

    /***************************************************************
    @author: yiheng
    @date: 2026/6/1
    @description: 移除字符串两端的空白符
    @see:
    @param: s待处理字符串
    @return: 返回处理后的字符串
    @fix:
    @update:
    ***************************************************************/
    QString TrimmedStr(const QString &s);

    /***************************************************************
    @author: yiheng
    @date: 2026/6/1
    @description: 将字符串中的旧字符串oldStr替换为新字符串newStr
    @see:
    @param: s待处理字符串
    @param: oldStr, 待替换的字符串
    @param: newStr, 需要替换为的新字符串
    @param: bReplaceOne, 是否只替换第一个匹配的字符串
    @param: cs, 是否区分大小写
    @return:
    @fix:
    @update:
    ***************************************************************/
    QString ReplaceStr(const QString &s, const QString& oldStr, const QString & newStr, bool bReplaceOne = false, Qt::CaseSensitivity cs = Qt::CaseSensitive);


    //////////// 加解密
    QString encrypt(const QString &plain, const QString &key);
    QString decrypt(const QString &cipherBase64, const QString &key);


    //////////// 文件处理



    /////////// 日期处理



}