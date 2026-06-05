#include "../stdafx.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../core/networkmanager.h"

#include <QScreen>
#include <QPushButton>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pNetWorkMgr(nullptr)
{
    ui->setupUi(this);
    m_pNetWorkMgr = NetworkManager::GetInstance(this);
    // QTimer* timer = new QTimer(this);
    // connect(timer, &QTimer::timeout, this, [&](){
    //     if (m_pNetWorkMgr)
    //     {
    //         if (m_pNetWorkMgr->IsOnline())
    //             ui->statusBar->showMessage(tr("Connected."));
    //         else
    //         {
    //            ui->statusBar->showMessage(tr("Disconnected."));
    //            if (m_pNetWorkMgr && !m_pNetWorkMgr->IsOnline()) m_pNetWorkMgr->ConnectToServer(SERVER_IP, LINSTEN_PORT);
    //         }
    //     }
    // });
    // timer->start(3000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitWindow()
{
    setWindowTitle(tr("Welcome"));
    ui->statusBar->showMessage(tr("Connected."));
    ui->lab_username->setText(tr("Welcome USER: %1").arg(m_sUserName));
}

bool MainWindow::ShowMainWindow(QWidget *parent, const QString& sUserName)
{
    MainWindow* win = new MainWindow(parent);;
    if(win)
    {
        win->m_sUserName = sUserName;
        win->InitWindow();
        QRect screenRect = QGuiApplication::primaryScreen()->geometry();
        win->move((screenRect.width() - win->width()) / 2, (screenRect.height() - win->height()) / 2);//移动到所在屏幕中间
        win->show();
    }
    else
    {
        return false;
    }

    return true;
}
