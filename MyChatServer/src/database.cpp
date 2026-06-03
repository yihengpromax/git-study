#include "database.h"
#include <QCryptographicHash>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

static QSqlDatabase db;
bool Database::Init(const QString &host, const QString &dbname,
                    const QString &user, const QString &pass)
{
    db = QSqlDatabase::addDatabase("QODBC", "conn1");
    db.setDatabaseName(
        QString("DRIVER={MySQL ODBC 9.7 Unicode Driver};"
                "SERVER=%1;DATABASE=%2;USER=%3;PASSWORD=%4;CHARSET=utf8mb4;")
            .arg(host, dbname, user, pass));
    if (!db.open())
    {
        qDebug() << "DB error:" << db.lastError().text();
        return false;
    }

    // 创建表（如果不存在）
    QSqlQuery query(QSqlDatabase::database("conn1")); // 当只存在一个数据库连接时，参数QSqlDatabase::database("conn1")可以省略

    // users
    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "id INT AUTO_INCREMENT PRIMARY KEY,"
               "username VARCHAR(32) UNIQUE NOT NULL,"
               "password_hash CHAR(64) NOT NULL,"
               "salt CHAR(32) NOT NULL,"
               "nickname VARCHAR(64),"
               "sex INT NOT NULL,"
               "birth VARCHAR(16) NOT NULL,"
               "signature VARCHAR(128),"
               "online BOOLEAN DEFAULT FALSE)");

    // friends
    query.exec("CREATE TABLE IF NOT EXISTS friends ("
               "user_id INT, friend_id INT,"
               "PRIMARY KEY(user_id, friend_id))");

    // offline_messages
    query.exec("CREATE TABLE IF NOT EXISTS offline_messages ("
               "id INT AUTO_INCREMENT PRIMARY KEY,"
               "from_id INT, to_id INT, content TEXT, timestamp DATETIME)");

    return true;
}

static QString HashPassword(const QString &pwd, const QString &salt)
{
    QByteArray combined = (pwd + salt).toUtf8();
    return QCryptographicHash::hash(combined, QCryptographicHash::Sha256).toHex();
}

bool Database::UserLogin(const QString &username, const QString &password, int &userId)
{
    QSqlQuery q(QSqlDatabase::database("conn1"));
    q.prepare("SELECT id, password_hash, salt FROM users WHERE username = :name");
    q.bindValue(":name", username);
    if (!q.exec() || !q.next()) return false;
    QString hashStored = q.value(1).toString();
    QString salt = q.value(2).toString();
    if (HashPassword(password, salt) != hashStored) return false;
    userId = q.value(0).toInt();
    UpdateUserOnline(userId, true);

    return true;
}

bool Database::UserRegister(const QString &username, const QString &password, const QString &nickname, int sex, const QString &birth, const QString &signature)
{
    QString salt = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    salt = QCryptographicHash::hash(salt.toUtf8(), QCryptographicHash::Sha256).toHex().left(32);
    QString pwdHash = HashPassword(password, salt);
    QSqlQuery q(QSqlDatabase::database("conn1"));
    q.prepare("INSERT INTO users (username, password_hash, salt, nickname, sex, birth, signature) VALUES (:u, :h, :s, :n, :sex, :brith, :signature)");
    q.bindValue(":u", username);
    q.bindValue(":h", pwdHash);
    q.bindValue(":s", salt);
    q.bindValue(":n", nickname);
    q.bindValue(":sex", sex);
    q.bindValue(":birth", birth);
    q.bindValue(":signature", signature);

    return q.exec();
}

QList<UserInfo> Database::GetFriendList(int userId)
{
    QList<UserInfo> list;
    QSqlQuery q(QSqlDatabase::database("conn1"));
    q.prepare("SELECT u.id, u.username, u.nickname, u.online FROM users u "
              "JOIN friends f ON (f.friend_id = u.id) WHERE f.user_id = :uid");
    q.bindValue(":uid", userId);
    if (!q.exec()) return list;
    while (q.next()) {
        UserInfo info;
        info.id = q.value(0).toInt();
        info.username = q.value(1).toString();
        info.nickname = q.value(2).toString();
        info.online = q.value(3).toBool();
        list.append(info);
    }

    return list;
}

bool Database::AddFriend(int userId, int friendId)
{
    QSqlQuery q(QSqlDatabase::database("conn1"));
    q.prepare("INSERT IGNORE INTO friends (user_id, friend_id) VALUES (:u, :f)");
    q.bindValue(":u", userId);
    q.bindValue(":f", friendId);

    return q.exec();
}

bool Database::StoreOfflineMsg(int fromId, int toId, const QString &content) {
    QSqlQuery q(QSqlDatabase::database("conn1"));
    q.prepare("INSERT INTO offline_messages (from_id, to_id, content, timestamp) "
              "VALUES (:f, :t, :c, NOW())");
    q.bindValue(":f", fromId);
    q.bindValue(":t", toId);
    q.bindValue(":c", content);

    return q.exec();
}

QList<QStringList> Database::GetOfflineMsgs(int userId)
{
    QList<QStringList> msgs;
    QSqlQuery q(QSqlDatabase::database("conn1"));
    q.prepare("SELECT from_id, content, timestamp FROM offline_messages WHERE to_id = :uid");
    q.bindValue(":uid", userId);
    if (!q.exec()) return msgs;
    while (q.next()) {
        QStringList msg;
        msg << QString::number(q.value(0).toInt()) << q.value(1).toString() << q.value(2).toString();
        msgs.append(msg);
    }

    return msgs;
}

bool Database::ClearOfflineMsgs(int userId)
{
    QSqlQuery q(QSqlDatabase::database("conn1"));
    q.prepare("DELETE FROM offline_messages WHERE to_id = :uid");
    q.bindValue(":uid", userId);

    return q.exec();
}

void Database::UpdateUserOnline(int userId, bool online)
{
    QSqlQuery q(QSqlDatabase::database("conn1"));
    q.prepare("UPDATE users SET online = :o WHERE id = :id");
    q.bindValue(":o", online);
    q.bindValue(":id", userId);
    q.exec();
}

int Database::GetUserIdByUsername(const QString &username)
{
    QSqlQuery q(QSqlDatabase::database("conn1"));
    q.prepare("SELECT id FROM users WHERE username = :name");
    q.bindValue(":name", username);
    if (q.exec() && q.next()) return q.value(0).toInt();

    return -1;
}