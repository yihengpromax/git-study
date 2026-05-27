#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>

namespace Ui {
class LoginDlg;
}

class LoginDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDlg(QWidget *parent = nullptr);
    ~LoginDlg();

    static bool ShowLoginDlg(QWidget *parent = nullptr);
    void InitDlg();
    void InitSignalWithSlot();

private slots:
    void OnBtnLoginClicked();
    void OnBtnRegisterClicked();
    void OnBtnModifyPassword();

private:
    Ui::LoginDlg *ui;
};

#endif // LOGINDLG_H
