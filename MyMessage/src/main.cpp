#include "ui/logindlg.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginDlg dlg;
    dlg.show();

    return a.exec();
}
