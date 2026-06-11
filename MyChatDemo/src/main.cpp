#include "ui/loginframe.h"
// #include "utils/util.h"
#include "utils/resourceloader.h"
#include "core/networkmanager.h"

#include <QApplication>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(ResourceLoader::loadIconResouse(":/resources/icon/MyChat.ico"));
    app.setStyleSheet(ResourceLoader::loadTextResource(":/resources/GlobalStyle.qss"));
    app.setQuitOnLastWindowClosed(true);

    // Test
    // QStringList sLst = Util::SplitStr("abcgdefgghjgkl", "g");
    // qDebug() << sLst;

    LoginFrame* frame = LoginFrame::ShowLoginFrame(nullptr);
    int exitcode = 0;
    if (frame)
    {
        frame->show();

        // 不要把 QTcpSocket 对象移动到工作线程，而是让 socket 对象本身属于工作线程
        Network::NetworkManager* pNetworkMgr = Network::GetInstance();
        QThread thNetwork;
        QObject::connect(&thNetwork, &QThread::started, pNetworkMgr, &Network::NetworkManager::InitNetwork);
        pNetworkMgr->moveToThread(&thNetwork); // 先移动到子线程，那么之后创建的对象都隶属于子线程，亲和性
        thNetwork.start();

        // 进入Qt事件循环
        exitcode = app.exec();
        thNetwork.quit();
        if (!thNetwork.wait(3000))
        {
            thNetwork.terminate();
        }
    }

    return exitcode;
}
