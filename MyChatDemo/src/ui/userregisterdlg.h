#ifndef USERREGISTERDLG_H
#define USERREGISTERDLG_H

#include <QDialog>

namespace Ui {
class UserRegisterDlg;
}

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
    void OnBtnBoxClicked();
    void OnLineEditValueChanged();

private:
    Ui::UserRegisterDlg *ui;
};

#endif // USERREGISTERDLG_H
