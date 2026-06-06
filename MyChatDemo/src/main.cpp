#include "ui/loginframe.h"
#include "utils/util.h"
#include "utils/resourceloader.h"
#include "core/networkmanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(ResourceLoader::loadIconResouse(":/resources/icon/MyChat.ico"));
    app.setStyleSheet(ResourceLoader::loadTextResource(":/resources/GlobalStyle.qss"));
    app.setQuitOnLastWindowClosed(true);

    // Test
    QStringList sLst = Util::SplitStr("abcgdefgghjgkl", "g");
    qDebug() << sLst;

    LoginFrame* frame = LoginFrame::ShowLoginFrame(nullptr);
    int exitcode = 0;
    if (frame)
    {
        frame->show();
        exitcode = app.exec(); // 进入Qt事件循环

        delete frame;
        frame = nullptr;
    }

    return exitcode;
}
