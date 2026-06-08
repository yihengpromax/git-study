#include "../stdafx.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../core/networkmanager.h"

#include <QScreen>
#include <QPushButton>
#include <QTimer>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Network::NetworkManager *pNetworkMgr = Network::GetInstance();
    connect(this, &MainWindow::SendChat, pNetworkMgr, &Network::NetworkManager::SendChat);
    connect(ui->refreshButton, &QPushButton::clicked, pNetworkMgr, &Network::NetworkManager::SendGetFriendList);
    connect(pNetworkMgr, &Network::NetworkManager::FriendListReceived, this, &MainWindow::OnGetFriendList);
    connect(pNetworkMgr, &Network::NetworkManager::AddFriendResult, this, &MainWindow::OnAddFriendResult);
    connect(this, &MainWindow::SendAddFriend, pNetworkMgr, &Network::NetworkManager::AddFriend);
    connect(pNetworkMgr, &Network::NetworkManager::ChatMessageReceived, this, &MainWindow::OnChatMessageReceived);

    connect(pNetworkMgr, &Network::NetworkManager::UpdateConnState, this, [&](QTcpSocket::SocketState state){
        switch (state)
        {
        case QTcpSocket::SocketState::ConnectedState:
            ui->labStatus->setText(tr("Connected"));
            ui->sendButton->setEnabled(true);
            break;
        case QTcpSocket::SocketState::UnconnectedState:
            ui->labStatus->setText(tr("Disconnected"));
            ui->sendButton->setEnabled(false);
            break;
        case QTcpSocket::SocketState::ConnectingState:
            ui->labStatus->setText(tr("Connecting"));
            ui->sendButton->setEnabled(false);
            break;
        default:
            break;
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitWindow()
{
    setWindowTitle(tr("Welcome[%1]").arg(m_sUserName));
    ui->labStatus->setText(APP_VERSION);
    InitConnect();
}

void MainWindow::InitConnect()
{
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::OnBtnSendClicked);
    connect(ui->addFriendButton, &QPushButton::clicked, this, &MainWindow::OnBtnAddFriendClicked);
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
        emit SendChat(ui->cbFriendList->currentText(), ui->inputEdit->toPlainText());
        QString sCurTime = QDateTime::currentDateTime().toString();
        ui->messageDisplay->append("[" + sCurTime + "]" + m_sUserName + ":");
        ui->messageDisplay->append("\r" + ui->inputEdit->toPlainText() + "\n");

        ui->inputEdit->clear();
    }
}

void MainWindow::OnGetFriendList(const QJsonArray &friends)
{
    qDebug() << "//============================= friendlist ==========================";
    ui->cbFriendList->clear();
    for (const auto &obj : friends)
    {
        ui->cbFriendList->addItem(obj.toObject()["username"].toString());
    }
}

void MainWindow::OnAddFriendResult(bool ok, const QString& err)
{
    ok ? QMessageBox::information(this, tr("Tips"), tr("Add Success！")) : QMessageBox::information(this, tr("Tips"), tr("Add Faild"));
}

void MainWindow::OnBtnAddFriendClicked()
{
    if (m_sUserName.isEmpty())
        return;

    if (!ui->usernameInput->text().isEmpty())
    {
        emit SendAddFriend(m_sUserName, ui->usernameInput->text());
    }
    else
    {
        QMessageBox::information(this, tr("Tips"), tr("Friend Name is Empty!"));
    }
}

void MainWindow::OnChatMessageReceived(int fromId, const QString &fromUserName, const QString &content, qint64 timestamp)
{
    if (-1 == fromId)
        return;

    QDateTime dt1 = QDateTime::fromMSecsSinceEpoch(timestamp);
    ui->messageDisplay->append("[" + dt1.toString() + "]" + fromUserName + ":");
    ui->messageDisplay->append("\r" + content + "\n");
}
