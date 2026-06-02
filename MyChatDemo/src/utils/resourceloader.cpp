#include "resourceloader.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>

QString ResourceLoader::getResourcePath(const QString &resourcePath)
{
    QString relativePath = resourcePath;
    const QString exeDir = QCoreApplication::applicationDirPath();
    QString fsPath = QDir::cleanPath(exeDir + "/resource/" + relativePath.remove(":/res/"));
    if (QFile::exists(fsPath))
    {
        return fsPath;
    }

    // NOTE: 回退到资源文件
    return resourcePath;
}

QString ResourceLoader::loadTextResource(const QString &resourcePath)
{
    QFile file(getResourcePath(resourcePath));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Resource load failed:" << resourcePath;
        return "";
    }

    // NOTE: 统一转为字符串
    return QString::fromUtf8(file.readAll());
}

QIcon ResourceLoader::loadIconResouse(const QString &resourcePath)
{
    return QIcon(getResourcePath(resourcePath));
}

bool ResourceLoader::resourceExists(const QString &resourcePath)
{
    return QFile::exists(getResourcePath(resourcePath));
}

void *ResourceLoader::getUIPointer(const QString &resourcePath)
{
    // TODO
    return nullptr;
}
