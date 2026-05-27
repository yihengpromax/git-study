#include "logindlg.h"
#include "ui_logindlg.h"

LoginDlg::LoginDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDlg)
{
    ui->setupUi(this);
}

LoginDlg::~LoginDlg()
{
    delete ui;
}

bool LoginDlg::ShowLoginDlg(QWidget *parent)
{

}

void LoginDlg::InitDlg()
{

}

void LoginDlg::InitSignalWithSlot()
{

}

void LoginDlg::OnBtnLoginClicked()
{

}

void LoginDlg::OnBtnRegisterClicked()
{

}

void LoginDlg::OnBtnModifyPassword()
{

}


