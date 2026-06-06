#include "stdafx.h"
#include "server.h"
#include "database.h"
#include "protocol.h"
#include "utils/utils.h"

#include <QCoreApplication>
#include <thread>
#include <iostream>

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

    // NOTE: Init Commandline Input
    std::thread th([&](){
        std::string line;
        while(std::getline(std::cin, line))
        {
            if (0 == line.compare("bye"))
            {
                std::cout << "bye." << std::endl;
                app.quit();
            }
        }
    });

    // NOTE: InitServer
    ChatServer server;
    server.Start(LINSTEN_PORT);

    return app.exec();
}