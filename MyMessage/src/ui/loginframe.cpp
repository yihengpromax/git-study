#include "loginframe.h"
#include "ui_loginframe.h"

#include <QScreen>
#include <QMessageBox>

LoginFrame::LoginFrame(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginFrame)
{
    ui->setupUi(this);
}

LoginFrame::~LoginFrame()
{
    delete ui;
}

LoginFrame* LoginFrame::ShowLoginFrame(QWidget *parent)
{
    LoginFrame* loginWin = new LoginFrame(parent);
    if (loginWin)
    {
        loginWin->InitWindow();
        QRect screenRect = QGuiApplication::primaryScreen()->geometry();
        loginWin->move((screenRect.width() - loginWin->width()) / 2, (screenRect.height() - loginWin->height()) / 2);//移动到所在屏幕中间

        return loginWin;
    }

    return nullptr;
}

void LoginFrame::InitWindow()
{
    setWindowTitle("登录到");
    InitSignalWithSlot();
}

void LoginFrame::InitSignalWithSlot()
{
    // Button
    {
        connect(ui->btn_login, &QPushButton::clicked, this, &LoginFrame::OnBtnLoginClicked);
        connect(ui->btn_register, &QPushButton::clicked, this, &LoginFrame::OnBtnRegisterClicked);
        connect(ui->btn_change_password, &QPushButton::clicked, this, &LoginFrame::OnBtnModifyPassword);
    }

    // LineEdit
    {


    }
}

void LoginFrame::OnBtnLoginClicked()
{
    if (!ui->lineedit_name->text().isEmpty() && !ui->lineedit_password->text().isEmpty())
    {
        // 读取数据库数据，校验账号密码
    }
    else
    {
        QMessageBox::information(this, tr("Tips"), tr("Account or password is empty."));
    }
}

void LoginFrame::OnBtnRegisterClicked()
{
    QMessageBox::information(this, tr("Tips"), tr("To be developed."));
}

void LoginFrame::OnBtnModifyPassword()
{
    QMessageBox::information(this, tr("Tips"), tr("To be developed."));
}


