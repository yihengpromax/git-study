#include "../stdafx.h"
#include "loginframe.h"
#include "ui_loginframe.h"
#include "../utils/util.h"

#include <QScreen>
#include <QMessageBox>
#include <QSettings>
#include <QMap>

LoginFrame::LoginFrame(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginFrame)
    , m_bRememberPassword(false)
    , m_bAutoLogin(false)
    , m_sCurUserID("")
    , m_sCurPassword("")
    , m_sVersion("")
{
    ui->setupUi(this);
}

LoginFrame::~LoginFrame()
{
    SaveLocalConfig();
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
    setWindowTitle(tr("Login In"));
    ReadLocalConfig();
    InitUI();
    InitSignalWithSlot();

    // 处理自动登录事件
    if (m_bRememberPassword && m_bAutoLogin)
    {

    }
}

void LoginFrame::InitUI()
{
    // checkbox
    {
        ui->cb_auto_login->setChecked(m_bAutoLogin);
        ui->cb_remember_password->setChecked(m_bRememberPassword);
    }

    // lineedit
    {
        ui->lineedit_name->setText(m_sCurUserID);
        if (m_bRememberPassword) ui->lineedit_password->setText(m_sCurPassword);
    }
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

    // checkbox
    {
        // 记住密码
        connect(ui->cb_remember_password, &QCheckBox::checkStateChanged, this, &LoginFrame::OnCkbRememberStateChanged);

        // 自动登录
        connect(ui->cb_auto_login, &QCheckBox::checkStateChanged, this, &LoginFrame::OnCkbAutoLoginStateChanged);
    }
}

void LoginFrame::ReadLocalConfig()
{
    QString qsAppConfigPath = QApplication::applicationDirPath() + "/config.ini";
    QSettings localSet(qsAppConfigPath, QSettings::IniFormat); // Auto Created if is not exist.

    localSet.beginGroup("Main");
    m_sVersion = localSet.contains("Version") ? localSet.value("Version").toString(): APP_VERSION;
    m_sCurUserID = localSet.contains("UserID") ? localSet.value("UserID").toString(): "";
    QString qsPwd = localSet.contains("Password") ? localSet.value("Password").toString(): "";
    m_sCurPassword = Util::decrypt(qsPwd, MASTER_KEY);
    m_bRememberPassword = localSet.contains("RememberPassword") ? localSet.value("RememberPassword").toBool(): false;
    m_bAutoLogin = localSet.contains("AutoLogin") ? localSet.value("AutoLogin").toBool(): false;
    localSet.endGroup();
}

void LoginFrame::SaveLocalConfig()
{
    QString qsAppConfigPath = QApplication::applicationDirPath() + "/config.ini";
    QSettings localSet(qsAppConfigPath, QSettings::IniFormat);

    localSet.beginGroup("Main");
    localSet.setValue("Version", m_sVersion);
    localSet.setValue("Language", 0);
    localSet.setValue("UserID", m_sCurUserID);
    localSet.setValue("Password", Util::encrypt(m_sCurPassword, MASTER_KEY));
    localSet.setValue("RememberPassword", m_bRememberPassword);
    localSet.setValue("AutoLogin", m_bAutoLogin);

    localSet.endGroup();
}

void LoginFrame::AddGroupToConfig(const QString& qsConfName, const QString &qsGroupName, const QMap<QString, QVariant> &valMap)
{
    QSettings settings(qsConfName, QSettings::IniFormat);

    settings.beginGroup(qsGroupName);
    QMap<QString, QVariant>::ConstIterator it = valMap.begin();
    for (; it != valMap.end(); ++it)
    {
        settings.setValue(it.key(), it.value());
    }
    settings.endGroup();
}

void LoginFrame::OnBtnLoginClicked()
{
    if (!ui->lineedit_name->text().isEmpty() && !ui->lineedit_password->text().isEmpty())
    {
        // 读取数据库数据，校验账号密码
        // TODO

        m_sCurUserID = ui->lineedit_name->text();
        m_sCurPassword = ui->lineedit_password->text();
        QMessageBox::information(this, tr("Tips"), tr("Login Success!"));
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

void LoginFrame::OnCkbAutoLoginStateChanged(Qt::CheckState state)
{
    if (state == Qt::Checked)
    {
        m_bAutoLogin = true;
        // TODO
    }
    else
    {
        m_bAutoLogin = false;
        // TODO
    }
}

void LoginFrame::OnCkbRememberStateChanged(Qt::CheckState state)
{
    if (state == Qt::Checked)
    {
         m_bRememberPassword = true;
        // TODO
    }
    else
    {
        ui->cb_auto_login->setChecked(false);
        m_bRememberPassword = false;
        // TODO
    }
}


