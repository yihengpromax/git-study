#include "../stdafx.h"
#include "utils.h"

#include <QProcessEnvironment>
#include <QStandardPaths>
#include <iostream>

void Utils::printfRed(const char *s)
{
    printf("\033[0m\033[1;31m%s\n\033[0m", s);
}

void Utils::printfGreen(const char *s)
{
    printf("\033[0m\033[1;32m%s\n\033[0m", s);
}

void Utils::printfYellow(const char *s)
{
    printf("\033[0m\033[1;33m%s\n\033[0m", s);
}

Utils::Logger::Logger()
    : m_LogLevel(LogLevel::LevelInfo)
    , m_logMode(LogMode::PrintOnly)
{

}

Utils::Logger::~Logger()
{

}

Utils::Logger &Utils::Logger::GetLogger()
{
    static Logger instance;
    return instance;
}

void Utils::Logger::SetLogLevel(LogLevel level)
{
    if (level != m_LogLevel) m_LogLevel = level;
}

void Utils::Logger::SetLogMode(LogMode mode)
{
    if (mode != m_logMode) m_logMode = mode;
}

void Utils::Logger::Error(const QString& msg)
{
    if (m_LogLevel >= LogLevel::LevelError)
    {
        if (m_logMode == LogMode::PrintOnly || m_logMode == LogMode::Both)
            std::cout << "[ERROR]: " << msg.toUtf8().data() << std::endl;

        if (m_logMode == LogMode::FileOnly || m_logMode == LogMode::Both)
            WriteLogToLocalFile(LogLevel::LevelError, QIODevice::Append, {msg});
    }
}

void Utils::Logger::Warning(const QString& msg)
{
    if (m_LogLevel >= LogLevel::LevelWarning)
    {
        if (m_logMode == LogMode::PrintOnly || m_logMode == LogMode::Both)
            std::cout << "[WARNING]: " << msg.toUtf8().data() << std::endl;

        if (m_logMode == LogMode::FileOnly || m_logMode == LogMode::Both)
            WriteLogToLocalFile(LogLevel::LevelWarning, QIODevice::Append, {msg});
    }
}

void Utils::Logger::Info(const QString& msg)
{
    if (m_LogLevel >= LogLevel::LevelInfo)
    {
        if (m_logMode == LogMode::PrintOnly || m_logMode == LogMode::Both)
            std::cout << "[INFO]: " << msg.toUtf8().data() << std::endl;

        if (m_logMode == LogMode::FileOnly || m_logMode == LogMode::Both)
            WriteLogToLocalFile(LogLevel::LevelInfo, QIODevice::Append, {msg});
    }
}

void Utils::Logger::Debug(const QString& msg)
{
    if (m_LogLevel >= LogLevel::LevelDebug)
    {
        if (m_logMode == LogMode::PrintOnly || m_logMode == LogMode::Both)
            std::cout << "[DEBUG]: " << msg.toUtf8().data() << std::endl;

        if (m_logMode == LogMode::FileOnly || m_logMode == LogMode::Both)
            WriteLogToLocalFile(LogLevel::LevelDebug, QIODevice::Append, {msg});
    }
}

void Utils::Logger::Error(const QStringList& msgLst)
{
    if (m_LogLevel >= LogLevel::LevelError)
    {
        if (m_logMode == LogMode::PrintOnly || m_logMode == LogMode::Both)
        {
            for (auto &it : msgLst)
                std::cout << "[ERROR]: " << it.toUtf8().data() << std::endl;
        }

        if (m_logMode == LogMode::FileOnly || m_logMode == LogMode::Both)
            WriteLogToLocalFile(LogLevel::LevelError, QIODevice::Append, msgLst);
    }
}

void Utils::Logger::Warning(const QStringList& msgLst)
{
    if (m_LogLevel >= LogLevel::LevelWarning)
    {
        if (m_logMode == LogMode::PrintOnly || m_logMode == LogMode::Both)
        {
            for (auto &it : msgLst)
                std::cout << "[WARNING]: " << it.toUtf8().data() << std::endl;
        }

        if (m_logMode == LogMode::FileOnly || m_logMode == LogMode::Both)
            WriteLogToLocalFile(LogLevel::LevelWarning, QIODevice::Append, msgLst);
    }
}

void Utils::Logger::Info(const QStringList& msgLst)
{
    if (m_LogLevel >= LogLevel::LevelInfo)
    {
        if (m_logMode == LogMode::PrintOnly || m_logMode == LogMode::Both)
        {
            for (auto &it : msgLst)
                std::cout << "[INFO]: " << it.toUtf8().data() << std::endl;
        }

        if (m_logMode == LogMode::FileOnly || m_logMode == LogMode::Both)
            WriteLogToLocalFile(LogLevel::LevelInfo, QIODevice::Append, msgLst);
    }
}

void Utils::Logger::Debug(const QStringList& msgLst)
{
    if (m_LogLevel >= LogLevel::LevelDebug)
    {
        if (m_logMode == LogMode::PrintOnly || m_logMode == LogMode::Both)
        {
            for (auto &it : msgLst)
                std::cout << "[DEBUG]: " << it.toUtf8().data() << std::endl;
        }

        if (m_logMode == LogMode::FileOnly || m_logMode == LogMode::Both)
            WriteLogToLocalFile(LogLevel::LevelDebug, QIODevice::Append, msgLst);
    }
}

void Utils::Logger::InitLogger(LogLevel level, LogMode mode)
{
    SetLoggerRule(level, mode);
    QString startLog = QString("Version: %1  Time: %2").arg(APP_VERSION).arg(GetCurrentDateTime());
    Info({startLog});
}

void Utils::Logger::SetLoggerRule(LogLevel level, LogMode mode)
{
    SetLogLevel(level);
    SetLogMode(mode);
}


// ===================================== Function ====================================
QString Utils::GetAppDataDirPath(const QString &defaultPath)
{
    QString path;
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    dir.cdUp();
    path = dir.path();
    if (!dir.exists())
    {
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
#if defined (QT_OS_WIN)
        path = env.value("APPDATA", defaultPath);
#else
        path = env.value("XDG_DATA_HOME", defaultPath);
#endif

    }

    return path;
}

bool Utils::WriteLogToLocalFile(Utils::LogLevel logLevel, QIODevice::OpenModeFlag qFlag, QStringList sLogLst)
{
    QString sLogPath = GetAppDataDirPath("") + "/MyChatServer";
    QDir dir(sLogPath);
    if (!dir.exists())
    {
        dir.mkdir(sLogPath);
    }

    QString sLogFile = GetAppDataDirPath("") + QString("/MyChatServer/%1(%2).log").arg(GetCurrentDate()).arg(APP_VERSION);
    QFile fi(sLogFile);
    if (!fi.open(qFlag))
    {
        return false;
    }

    QString sLogLevel;
    switch (logLevel)
    {
    case Utils::LevelDebug:sLogLevel = "[DEBUG]"; break;
    case Utils::LevelInfo:sLogLevel = "[INFO]"; break;
    case Utils::LevelWarning:sLogLevel = "[WARNING]"; break;
    case Utils::LevelError:sLogLevel = "[ERROR]"; break;
    default:break;
    }

    QTextStream out(&fi);
    for (const QString& sLog : sLogLst)
    {
        out << sLogLevel << sLog << "\n";
    }

    fi.close();

    return true;
}

QString Utils::GetCurrentDateTime()
{
    QString sDateTime;
    QDateTime dateTime = QDateTime::currentDateTime();
    sDateTime = dateTime.toString("yyyy-MM-dd hh:mm:ss");

    return sDateTime;
}

QString Utils::GetCurrentDate()
{
    QString sDate;
    QDate date = QDate::currentDate();
    sDate = date.toString("yyyy-MM-dd");

    return sDate;
}


