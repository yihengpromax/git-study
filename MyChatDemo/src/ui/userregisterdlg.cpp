#include "../stdafx.h"
#include "userregisterdlg.h"
#include "ui_userregisterdlg.h"
#include "../core/networkmanager.h"
#include "loadingbubbledialog.h"

#include <QMessageBox>
#include <QPushButton>
#include <QWidgetAction>

UserRegisterDlg::UserRegisterDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UserRegisterDlg)
    , m_bEnsurePasswordOk(false)
    , m_bOnline(false)
{
    ui->setupUi(this);

    // m_pNetworkMgr
    Network::NetworkManager* pNetworkMgr = Network::GetInstance();
    connect(this, &UserRegisterDlg::Registered, pNetworkMgr, &Network::NetworkManager::SendRegisterUser);
    connect(pNetworkMgr, &Network::NetworkManager::RegisterResult, this, &UserRegisterDlg::OnRegisterResult);
    connect(pNetworkMgr, &Network::NetworkManager::ErrorOccurred, this, [&](const QString& error){
        LoadingBubbleDialog* instanceDlg = LoadingBubbleDialog::GetInstance();
        if (instanceDlg->IsStarted()) instanceDlg->stopLoading();
        QMessageBox::critical(this, tr("Error"), tr("Encountered an error: %1, Please Retry").arg(error));
    });
    connect(pNetworkMgr, &Network::NetworkManager::UpdateConnState, this, [&](int state){
        m_bOnline = (state == QTcpSocket::ConnectedState);
        LoadingBubbleDialog* instanceDlg = LoadingBubbleDialog::GetInstance();
        if (m_bOnline)
        {
            if (instanceDlg->IsStarted()) instanceDlg->stopLoading();
        }

        if (404 == state)
        {

            if (instanceDlg->IsStarted()) instanceDlg->stopLoading();
            QMessageBox::critical(this, tr("Error"), tr("Connect Server Request Timeout, Please Retry"));
        }
    });
}

UserRegisterDlg::~UserRegisterDlg()
{
    delete ui;
}

void UserRegisterDlg::InitWindow()
{
    setWindowTitle("RegisterPage");
    InitUIAttribute();
    InitConnect();
}

void UserRegisterDlg::InitUIAttribute()
{
    // LineEdit
    {
        // password
        {
            ui->lineedit_password->setMaxLength(MAX_PASSWORD_LEN);
            ui->lineedit_password->setEchoMode(QLineEdit::Password);
            ui->lineedit_password->addAction(QIcon(":/resources/icon/right.png"), QLineEdit::TrailingPosition);
            QAction *actEye = ui->lineedit_password->addAction(QIcon(":/resources/icon/eye.png"), QLineEdit::TrailingPosition);
            QAction *actEyeHide = ui->lineedit_password->addAction(QIcon(":/resources/icon/eye_hide.png"), QLineEdit::TrailingPosition);

            ui->lineedit_password->actions().at(0)->setVisible(false);
            ui->lineedit_password->actions().at(1)->setVisible(false);

            connect(actEye, &QAction::triggered, this, [&](){
                ui->lineedit_password->actions().at(1)->setVisible(false);
                ui->lineedit_password->actions().at(2)->setVisible(true);
                ui->lineedit_password->setEchoMode(QLineEdit::Password);

            });
            connect(actEyeHide, &QAction::triggered, this, [&](){
                ui->lineedit_password->actions().at(1)->setVisible(true);
                ui->lineedit_password->actions().at(2)->setVisible(false);
                ui->lineedit_password->setEchoMode(QLineEdit::Normal);
            });
        }

        // enpassword
        {
            ui->lineedit_enpassword->setMaxLength(MAX_PASSWORD_LEN);
            ui->lineedit_enpassword->setEchoMode(QLineEdit::Password);
            ui->lineedit_enpassword->addAction(QIcon(":/resources/icon/right.png"), QLineEdit::TrailingPosition);

            QAction *actEye = ui->lineedit_enpassword->addAction(QIcon(":/resources/icon/eye.png"), QLineEdit::TrailingPosition);
            QAction *actEyeHide = ui->lineedit_enpassword->addAction(QIcon(":/resources/icon/eye_hide.png"), QLineEdit::TrailingPosition);

            ui->lineedit_enpassword->actions().at(0)->setVisible(false);
            ui->lineedit_enpassword->actions().at(1)->setVisible(false);

            connect(actEye, &QAction::triggered, this, [&](){
                ui->lineedit_enpassword->actions().at(1)->setVisible(false);
                ui->lineedit_enpassword->actions().at(2)->setVisible(true);
                ui->lineedit_enpassword->setEchoMode(QLineEdit::Password);

            });
            connect(actEyeHide, &QAction::triggered, this, [&](){
                ui->lineedit_enpassword->actions().at(1)->setVisible(true);
                ui->lineedit_enpassword->actions().at(2)->setVisible(false);
                ui->lineedit_enpassword->setEchoMode(QLineEdit::Normal);
            });
        }

        // username
        ui->lineedit_username->setMaxLength(MAX_USERNAME_LEN);
        ui->lineedit_username->addAction(QIcon(":/resources/icon/right.png"), QLineEdit::TrailingPosition);
        ui->lineedit_username->actions().at(0)->setVisible(false);

        // nickname
        ui->lineedit_nickname->setMaxLength(MAX_NICKNAME_LEN);
        ui->lineedit_nickname->addAction(QIcon(":/resources/icon/right.png"), QLineEdit::TrailingPosition);
        ui->lineedit_nickname->actions().at(0)->setVisible(false);
    }

    // dateedit
    ui->dateedit_birth->setMaximumDate(QDate(2026, 6, 4));
    ui->dateedit_birth->setMinimumDate(QDate(1826, 6, 4));
}

void UserRegisterDlg::InitConnect()
{
    // Button
    {
        connect(ui->btn_register, &QPushButton::clicked, this, &UserRegisterDlg::OnBtnAcceptClicked);
    }

    // LineEdit
    {
        connect(ui->lineedit_username, &QLineEdit::textChanged, this, &UserRegisterDlg::OnLineEditTextChanged);
        connect(ui->lineedit_nickname, &QLineEdit::textChanged, this, &UserRegisterDlg::OnLineEditTextChanged);
        connect(ui->lineedit_password, &QLineEdit::textChanged, this, &UserRegisterDlg::OnLineEditTextChanged);
        connect(ui->lineedit_enpassword, &QLineEdit::textChanged, this, &UserRegisterDlg::OnLineEditTextChanged);
    }

    // TextEdit
    connect(ui->textedit_signatrue, &QPlainTextEdit::textChanged, this, [&](){
        QString text = ui->textedit_signatrue->toPlainText();
        if (text.length() > MAX_SIGTEXT_LEN)
        {
            ui->textedit_signatrue->setPlainText(text.left(MAX_SIGTEXT_LEN));
            QTextCursor cursor = ui->textedit_signatrue->textCursor();
            cursor.movePosition(QTextCursor::End);
            ui->textedit_signatrue->setTextCursor(cursor);
        }
    });
}

bool UserRegisterDlg::ShowUserRegisterDlg(QWidget *parent)
{
    bool bRst = false;
    UserRegisterDlg* dlg = new UserRegisterDlg(parent);
    if (dlg)
    {
        dlg->InitWindow();
        if (dlg->exec() == QDialog::Accepted)
        {
            // TODO
            bRst = true;
        }
        else
        {

        }

        delete dlg;
        dlg = nullptr;
    }

    return bRst;
}

void UserRegisterDlg::OnBtnAcceptClicked()
{
    // NOTE: 检验用户输入是否合格
    if (ui->lineedit_username->text().isEmpty() || ui->lineedit_nickname->text().isEmpty())
    {
        QMessageBox::information(this, tr("Tips"), tr("The username or nickname cannot be empty"));
        return;
    }

    if (!ui->lineedit_password->actions().at(0)->isVisible() || !ui->lineedit_enpassword->actions().at(0)->isVisible())
    {
        QMessageBox::information(this, tr("Tips"), tr("The Password is critical."));
        return;
    }

    LoadingBubbleDialog* instanceDlg = LoadingBubbleDialog::GetInstance();
    if (IsOnline())
    {
        // NOTE: 发送注册指令并等待结果
        instanceDlg->setText(tr("Please wait..."));
        if (!instanceDlg->IsStarted()) instanceDlg->startLoading();
        emit Registered(ui->lineedit_username->text(), ui->lineedit_enpassword->text(),
                        ui->lineedit_nickname->text(), ui->cb_sex->currentIndex(),
                        ui->dateedit_birth->date().toString(), ui->textedit_signatrue->toPlainText());
    }
    else
    {
        instanceDlg->setText(tr("Please Wait Connected..."));
        if (!instanceDlg->IsStarted()) instanceDlg->startLoading();
    }
}

void UserRegisterDlg::OnBtnRejectClicked()
{
    reject();
}

void UserRegisterDlg::OnLineEditTextChanged()
{
    QLineEdit *pLineedit = qobject_cast<QLineEdit*>(sender());
    if (pLineedit == ui->lineedit_username)
    {
        if (!ui->lineedit_username->text().isEmpty())
            ui->lineedit_username->actions().at(0)->setVisible(true);
        else
            ui->lineedit_username->actions().at(0)->setVisible(false);
    }
    else if (pLineedit == ui->lineedit_password)
    {
        if (ui->lineedit_password->text().length() >= 8 && HasContiansTwoTypeChar(ui->lineedit_password->text()))
            ui->lineedit_password->actions().at(0)->setVisible(true);
        else
            ui->lineedit_password->actions().at(0)->setVisible(false);
    }
    else if (pLineedit == ui->lineedit_enpassword)
    {
        if ((0 == ui->lineedit_enpassword->text().compare(ui->lineedit_password->text())) && !ui->lineedit_enpassword->text().isEmpty())
            ui->lineedit_enpassword->actions().at(0)->setVisible(true);
        else
            ui->lineedit_enpassword->actions().at(0)->setVisible(false);
    }
    else if (pLineedit == ui->lineedit_nickname)
    {
        if (!ui->lineedit_nickname->text().isEmpty())
            ui->lineedit_nickname->actions().at(0)->setVisible(true);
        else
            ui->lineedit_nickname->actions().at(0)->setVisible(false);
    }
}

void UserRegisterDlg::OnRegisterResult(bool bOk, const QString& err)
{
    LoadingBubbleDialog* instance = LoadingBubbleDialog::GetInstance();
    if (instance->IsStarted()) instance->stopLoading();
    if (bOk)
    {
        QMessageBox::information(this, tr("Tips"), tr("Register Ok!"));
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), tr("Register Failed. Error: %1").arg(err));
    }
}

bool UserRegisterDlg::HasContiansTwoTypeChar(const QString &sText)
{
    bool bHasNumber = false, bHasLetter = false, bHasSymbol = false;
    for (const QChar &c : sText)
    {
        if (c.isNumber())
            bHasNumber = true;
        if (c.isMark())
            bHasSymbol = true;
        if (c.isLetter())
            bHasLetter = true;
    }

    return (bHasLetter + bHasNumber + bHasSymbol) >= 2;
}

bool UserRegisterDlg::IsOnline()
{
    return m_bOnline;
}
