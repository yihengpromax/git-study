#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QList>

struct UserInfo
{
    int id;
    QString username;
    QString nickname;
    bool online;
};

class Database
{
public:
    static bool Init(const QString &host, const QString &dbname,
                     const QString &user, const QString &pass);
    static bool UserLogin(const QString &username, const QString &password, int &userId, QString& err);
    static bool UserRegister(const QString &username, const QString &password, const QString &nickname,
                             int sex, const QString &birth, const QString &signature, QString& err);
    static QList<UserInfo> GetFriendList(int userId, QString& err);
    static bool AddFriend(int userId, int friendId, QString& err);
    static bool StoreOfflineMsg(int fromId, int toId, const QString &content, QString& err);
    static QList<QStringList> GetOfflineMsgs(int userId, QString& err);
    static bool ClearOfflineMsgs(int userId, QString& err);
    static void UpdateUserOnline(int userId, bool online, QString& err);
    static int GetUserIdByUsername(const QString &username, QString& err);
};

#endif // DATABASE_H