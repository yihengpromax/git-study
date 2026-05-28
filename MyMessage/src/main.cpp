#include "ui/loginframe.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
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
