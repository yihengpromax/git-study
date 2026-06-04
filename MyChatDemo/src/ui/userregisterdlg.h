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

private slots:
    void OnBtnAcceptClicked();
    void OnBtnRejectClicked();
    void OnLineEditTextChanged();
    void OnRegisterResult(bool bOk, const QString& err);

private:
    bool HasContiansTwoTypeChar(const QString& sText);

private:
    Ui::UserRegisterDlg *ui;
    NetworkManager* m_pNetworkMgr;
    bool m_bEnsurePasswordOk;
};

#endif // USERREGISTERDLG_H
