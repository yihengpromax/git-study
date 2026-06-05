#include "stdafx.h"
#include "server.h"
#include "database.h"
#include "protocol.h"
#include "utils/utils.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // NOTE: InitLogger
    Utils::Logger::GetLogger().InitLogger(Utils::LogLevel::LevelInfo, Utils::LogMode::Both);

    // NOTE: InitDatabase
    if (!Database::Init("localhost", "mychatdb", "root", "123456"))
    {
        Utils::Logger::GetLogger().Error("Database init failed...");
        Utils::Logger::GetLogger().Info("Available drivers: ");
        Utils::Logger::GetLogger().Info(QSqlDatabase::drivers());
        return 1;
    }

    // NOTE: InitServer
    ChatServer server;
    server.Start(LINSTEN_PORT);
    return app.exec();
}