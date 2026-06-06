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

    // NOTE: InitServer
    ChatServer server;
    server.Start(LINSTEN_PORT);

    // NOTE: Init Commandline Input
    std::thread th([&](){
        std::string line;
        while(std::getline(std::cin, line))
        {
            if (0 == line.compare("bye"))
            {
                std::cout << "bye." << std::endl;
                app.quit(); // 跨线程操作
                break;
            }
            else if(0 == line.compare("version"))
            {
                std::cout << APP_VERSION << std::endl;
            }
            else if(0 == line.compare("help"))
            {
                std::cout << R"(help        Print HelpInfo.
                                version     Print Version.
                                bye         exit app.)" << std::endl;
            }
        }
    });
    int exitcode = app.exec();
    if (th.joinable()) th.join();

    return exitcode;
}