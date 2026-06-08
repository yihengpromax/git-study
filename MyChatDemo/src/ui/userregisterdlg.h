#ifndef USERREGISTERDLG_H
#define USERREGISTERDLG_H

#include <QDialog>

namespace Ui {
class UserRegisterDlg;
}

class NetworkManager;
class UserRegisterDlg : public QDialog
{
    Q_OBJECT

public:
    explicit UserRegisterDlg(QWidget *parent = nullptr);
    ~UserRegisterDlg();
    void InitWindow();
    void InitUIAttribute();
    void InitConnect();

public:
    static bool ShowUserRegisterDlg(QWidget *parent = nullptr);

signals:
    void Registered(const QString &username, const QString &password, const QString &nickname, int sex,
                    const QString &birth, const QString &signature);

private slots:
    void OnBtnAcceptClicked();
    void OnBtnRejectClicked();
    void OnLineEditTextChanged();
    void OnRegisterResult(bool bOk, const QString& err);

private:
    bool HasContiansTwoTypeChar(const QString& sText);
    bool IsOnline();

private:
    Ui::UserRegisterDlg *ui;
    bool m_bEnsurePasswordOk;
    bool m_bOnline;
};

#endif // USERREGISTERDLG_H
