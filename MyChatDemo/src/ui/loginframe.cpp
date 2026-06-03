#include "../stdafx.h"
#include "../protocol.h"
#include "./loginframe.h"
#include "./mainwindow.h"
#include "ui_loginframe.h"
#include "../utils/util.h"
#include "../core/networkmanager.h"
#include "loadingbubbledialog.h"
#include "userregisterdlg.h"

#include <QScreen>
#include <QMessageBox>
#include <QSettings>
#include <QMap>
#include <QTimer>

LoginFrame::LoginFrame(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginFrame)
    , m_bRememberPassword(false)
    , m_bAutoLogin(false)
    , m_pNetworkMgr(nullptr)
{
    ui->setupUi(this);

    // m_pNetworkMgr
    m_pNetworkMgr = NetworkManager::GetInstance(this);
    connect(m_pNetworkMgr, &NetworkManager::Connected, this, &LoginFrame::OnConnectedServer);
    connect(m_pNetworkMgr, &NetworkManager::Disconnected, this, &LoginFrame::OnDisconnectedServer);
    connect(m_pNetworkMgr, &NetworkManager::LoginResult, this, &LoginFrame::OnLoginResult);
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
        // 遍历配置
        QStringList sLst;
        for (auto &it : m_lstFirstGroupNames)
        {
            if (0 != it.compare("Main"))
            {
                sLst.push_back(it.remove("User_"));
            }
        }
        ui->combobox_id->addItems(sLst);
        ui->combobox_id->setCurrentText(m_sCurUserID);
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

    // combobox
    {
        connect(ui->combobox_id, &QComboBox::currentTextChanged, this, &LoginFrame::OnComboboxCurTextChanged);

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
    m_lstFirstGroupNames = localSet.childGroups();
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
    if (!ui->combobox_id->currentText().isEmpty() && !ui->lineedit_password->text().isEmpty())
    {
        m_sCurUserID = ui->combobox_id->currentText();
        m_sCurPassword = ui->lineedit_password->text();
        m_bRememberPassword = ui->cb_remember_password->isChecked();
        m_bAutoLogin = ui->cb_auto_login->isChecked();

        if (m_pNetworkMgr)
        {
            m_pNetworkMgr->ConnectToServer("127.0.0.1", LINSTEN_PORT);
            LoadingBubbleDialog& instance = LoadingBubbleDialog::GetInstance();
            if (!instance.IsStarted())
            {
                instance.setText("正在登录...");
                instance.startLoading();
            }
        }
        else
        {
            QMessageBox::information(this, tr("Tips"), tr("Can't Connect Server..."));
        }
    }
    else
    {
        QMessageBox::information(this, tr("Tips"), tr("Account or password is empty."));
    }
}

void LoginFrame::OnBtnRegisterClicked()
{
    UserRegisterDlg::ShowUserRegisterDlg(this);
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

void LoginFrame::OnComboboxCurTextChanged(const QString &sText)
{
    QString qsGroupName = "User_" + sText;
    QString qsAppConfigPath = QApplication::applicationDirPath() + "/config.ini";
    QSettings localSet(qsAppConfigPath, QSettings::IniFormat);
    localSet.beginGroup(qsGroupName);
    ui->lineedit_password->setText(Util::decrypt(localSet.value("Password").toString(), MASTER_KEY));
    ui->cb_remember_password->setChecked(localSet.value("RememberPassword").toBool());
    ui->cb_auto_login->setChecked(localSet.value("AutoLogin").toBool());
    localSet.endGroup();
}

void LoginFrame::OnLoginResult(bool ok, int userId)
{
    LoadingBubbleDialog& instance = LoadingBubbleDialog::GetInstance();
    if (instance.IsStarted()) instance.stopLoading();
    if (ok)
    {
        QString qsGroupName = "User_" + m_sCurUserID;
        QString qsAppConfigPath = QApplication::applicationDirPath() + "/config.ini";
        QSettings localSet(qsAppConfigPath, QSettings::IniFormat);
        localSet.beginGroup(qsGroupName);
        localSet.setValue("UserID", m_sCurUserID);
        localSet.setValue("UserName", m_sCurUserID);
        m_bRememberPassword ? localSet.setValue("Password", Util::encrypt(m_sCurPassword, MASTER_KEY)) :
            localSet.setValue("Password", "");
        localSet.setValue("RememberPassword", m_bRememberPassword);
        localSet.setValue("AutoLogin", m_bAutoLogin);
        localSet.endGroup();

        MainWindow::ShowMainWindow(this);
        this->deleteLater();
    }
    else
    {
        QMessageBox::information(this, tr("Tips"), tr("The account or password is incorrect."));
    }
}

void LoginFrame::OnConnectedServer()
{
    m_pNetworkMgr->SendLogin(m_sCurUserID, m_sCurPassword);
    qDebug() << "Connected...";
}

void LoginFrame::OnDisconnectedServer()
{
    qDebug() << "DisConnected...";
}


