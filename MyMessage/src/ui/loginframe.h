#ifndef LOGINFRAME_H
#define LOGINFRAME_H

#include <QMainWindow>

namespace Ui {
class LoginFrame;
}

class LoginFrame : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginFrame(QWidget *parent = nullptr);
    ~LoginFrame();
    void InitWindow();
    void InitUI();
    void InitSignalWithSlot();
    void ReadLocalConfig();
    void SaveLocalConfig();
    void AddGroupToConfig(const QString& qsConfName, const QString& qsGroupName, const QMap<QString, QVariant>& valMap);

public:
    static LoginFrame* ShowLoginFrame(QWidget *parent = nullptr);

private slots:
    void OnBtnLoginClicked();
    void OnBtnRegisterClicked();
    void OnBtnModifyPassword();
    void OnCkbAutoLoginStateChanged(Qt::CheckState state);
    void OnCkbRememberStateChanged(Qt::CheckState state);

private:
    Ui::LoginFrame *ui;
    bool m_bRememberPassword, m_bAutoLogin;
    QString m_sCurUserID, m_sCurPassword; // 明文
    QString m_sVersion;
};

#endif // LOGINFRAME_H
