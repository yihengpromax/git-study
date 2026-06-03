#include "stdafx.h"
#include "server.h"
#include "database.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // 初始化数据库连接（修改为你的MySQL配置）
    if (!Database::Init("localhost", "mychatdb", "root", "123456"))
    {
        qDebug() << "Available drivers:" << QSqlDatabase::drivers();
        qDebug() << "Database init failed...";
        return 1;
    }

    ChatServer server;
    server.Start(LINSTEN_PORT);
    return app.exec();
}