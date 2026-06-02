#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include <QString>
#include <QIcon>

class ResourceLoader
{
public:
    // NOTE: 将输入的相对路径转为绝对路径（优先文件系统）
    static QString getResourcePath(const QString &resourcePath);

    // NOTE: 加载资源文件（样式/翻译等）
    static QString loadTextResource(const QString &resourcePath);

    // NOTE: 加载图标（样式/翻译等）
    static QIcon loadIconResouse(const QString &resourcePath);

    // NOTE: 检查资源是否存在
    static bool resourceExists(const QString &resourcePath);

    // NOTE: 加载UI文件，返回唯一的ui指针如Ui_CalculatorFrm类指针，对标UI设计器的效果
    static void *getUIPointer(const QString &resourcePath);

};

#endif // RESOURCELOADER_H
