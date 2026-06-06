#include "../stdafx.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../core/networkmanager.h"

#include <QScreen>
#include <QPushButton>
#include <QTimer>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pNetWorkMgr(nullptr)
{
    ui->setupUi(this);
    m_pNetWorkMgr = NetworkManager::GetInstance();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitWindow()
{
    setWindowTitle(tr("Welcome[%1]").arg(m_sUserName));
    InitConnect();
}

void MainWindow::InitConnect()
{
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::OnBtnSendClicked);
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

void MainWindow::OnBtnSendClicked()
{
    if (!ui->inputEdit->toPlainText().isEmpty())
    {
        if (m_pNetWorkMgr && m_pNetWorkMgr->IsOnline())
        {
            m_pNetWorkMgr->SendChat("wed", ui->inputEdit->toPlainText());
            ui->messageDisplay->append(m_sUserName + ":");
            ui->messageDisplay->append(ui->inputEdit->toPlainText());
        }
        else
        {
            QMessageBox::critical(this, tr("Error"), tr("Can't Connect to Server."));
        }

        ui->inputEdit->clear();
    }
}
