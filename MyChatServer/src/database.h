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
    static bool UserLogin(const QString &username, const QString &password, int &userId);
    static bool UserRegister(const QString &username, const QString &password, const QString &nickname, int sex, const QString &birth, const QString &signature);
    static QList<UserInfo> GetFriendList(int userId);
    static bool AddFriend(int userId, int friendId);
    static bool StoreOfflineMsg(int fromId, int toId, const QString &content);
    static QList<QStringList> GetOfflineMsgs(int userId);
    static bool ClearOfflineMsgs(int userId);
    static void UpdateUserOnline(int userId, bool online);
    static int GetUserIdByUsername(const QString &username);
};

#endif // DATABASE_H