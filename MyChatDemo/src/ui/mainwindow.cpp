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
    connect(this, &MainWindow::SendChatReq, pNetworkMgr, &Network::NetworkManager::SendChat);
    connect(ui->refreshButton, &QPushButton::clicked, this, [&](){
        emit SendGetFriendListReq();
    });
    connect(this, &MainWindow::SendGetFriendListReq, pNetworkMgr, &Network::NetworkManager::SendGetFriendList);
    connect(pNetworkMgr, &Network::NetworkManager::FriendListReceived, this, &MainWindow::OnGetFriendList);
    connect(pNetworkMgr, &Network::NetworkManager::AddFriendResult, this, &MainWindow::OnAddFriendResult);
    connect(this, &MainWindow::SendAddFriendReq, pNetworkMgr, &Network::NetworkManager::AddFriend);
    connect(pNetworkMgr, &Network::NetworkManager::ChatMessageReceived, this, &MainWindow::OnChatMessageReceived);
    connect(this, &MainWindow::SendGetOfflineMsgReq, pNetworkMgr, &Network::NetworkManager::SendGetofflineMsgReq);

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
    ui->labVersion->setText(QString("%1@yiheng").arg(APP_VERSION));
    InitConnect();
    InitChatInfo();
}

void MainWindow::InitConnect()
{
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::OnBtnSendClicked);
    connect(ui->addFriendButton, &QPushButton::clicked, this, &MainWindow::OnBtnAddFriendClicked);
}

void MainWindow::InitChatInfo()
{
    if (!m_sUserName.isEmpty())
    {
        emit SendGetFriendListReq();
        emit SendGetOfflineMsgReq(m_sUserName);
    }
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
        emit SendChatReq(ui->cbFriendList->currentText(), ui->inputEdit->toPlainText());
        QString sCurTime = QDateTime::currentDateTime().toString();
        ui->messageDisplay->append(" [" + sCurTime + "] " + m_sUserName + ":");
        // appendBubble(ui->inputEdit->toPlainText(), true);
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
        emit SendAddFriendReq(m_sUserName, ui->usernameInput->text());
        ui->usernameInput->clear();
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
    ui->messageDisplay->append(" [" + dt1.toString() + "] " + fromUserName + ":");
    // appendBubble(content, false);
    ui->messageDisplay->append("\r" + content + "\n");
}

void MainWindow::appendBubble(const QString &text, bool isSelf)
{
    QString align = isSelf ? "right" : "left";
    QString bgColor = isSelf ? "#95ec69" : "#ffffff";
    QString html = QString(
                       "<div style='text-align: %1; margin: 8px;'>"
                       "  <div style='display: inline-block; max-width: 70%%; background-color: %2; "
                       "              border-radius: 8px; padding: 8px 12px;'>"
                       "    <span style='font-size: 14px; color: #111111; word-wrap: break-word;'>%3</span>"
                       "  </div>"
                       "</div>"
                       ).arg(align, bgColor, text.toHtmlEscaped());
    ui->messageDisplay->insertHtml(html);
}
