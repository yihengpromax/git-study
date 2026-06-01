#include "ui/loginframe.h"
#include "utils/util.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Test
    QStringList sLst = Util::SplitStr("abcgdefgghjgkl", "g");
    qDebug() << sLst;

    LoginFrame* frame = LoginFrame::ShowLoginFrame(nullptr);
    int exitcode = 0;
    if (frame)
    {
        frame->show();
        exitcode = a.exec(); // 进入Qt事件循环

        delete frame;
        frame = nullptr;
    }


    return exitcode;
}
