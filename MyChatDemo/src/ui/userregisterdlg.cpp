#include "userregisterdlg.h"
#include "ui_userregisterdlg.h"

UserRegisterDlg::UserRegisterDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UserRegisterDlg)
{
    ui->setupUi(this);
}

UserRegisterDlg::~UserRegisterDlg()
{
    delete ui;
}

void UserRegisterDlg::InitWindow()
{
    InitUIAttribute();
    InitConnect();
}

void UserRegisterDlg::InitUIAttribute()
{

}

void UserRegisterDlg::InitConnect()
{
    // Button
    {

    }

    // LineEdit
    {

    }
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

void UserRegisterDlg::OnBtnBoxClicked()
{

}

void UserRegisterDlg::OnLineEditValueChanged()
{

}
