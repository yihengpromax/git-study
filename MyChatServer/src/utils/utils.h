#pragma once
#include <QDir>

namespace Utils
{

enum LogLevel
{
    LevelError = 0, LevelWarning, LevelInfo, LevelDebug
};

enum LogMode
{
    PrintOnly = 0, FileOnly, Both
};

/***************************************************************
@author: yiheng
@date: 2026/6/5
@description: 获取appdata路径
@see:
@param: defaultPath，默认路径
@return: 返回appdata路径
@fix:
@update:
***************************************************************/
QString GetAppDataDirPath(const QString &defaultPath);

/***************************************************************
@author: yiheng
@date: 2026/6/5
@description: 使用Base64进行简易加密
@see:
@param: logLevel，日志等级
@param: qFlag，打开文件的模式
@param: sLogLst,日志内容
@return: 返回结果
@fix:
@update:
***************************************************************/
bool WriteLogToLocalFile(LogLevel logLevel, QIODevice::OpenModeFlag qFlag, QStringList sLogLst);

/***************************************************************
@author: yiheng
@date: 2026/6/5
@description: 获取当前系统时间
@see:
@return: 返回当前系统时间
@fix:
@update:
***************************************************************/
QString GetCurrentDateTime();
QString GetCurrentDate();

// ============================= 简易日志类 =============================
class Logger
{
public:
    static Logger& GetLogger();

public:
    void SetLogLevel(LogLevel level);
    void SetLogMode(LogMode mode);
    void Error(const QStringList& msgLst);
    void Warning(const QStringList& msgLst);
    void Info(const QStringList& msgLst);
    void Debug(const QStringList& msgLst);
    void Error(const QString& msg);
    void Warning(const QString& msg);
    void Info(const QString& msg);
    void Debug(const QString& msg);
    void InitLogger(LogLevel level, LogMode mode);
    void SetLoggerRule(LogLevel level, LogMode mode);

private:
    explicit Logger();
    ~Logger();

private:
    LogLevel m_LogLevel;
    LogMode m_logMode;
};

} // namespace Utils
