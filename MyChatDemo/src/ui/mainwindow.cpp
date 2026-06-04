#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScreen>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitWindow()
{
    setWindowTitle(tr("Welcome"));
    ui->lab_username->setText(tr("Welcome USER: %1").arg(m_sUserName));
}

bool MainWindow::ShowMainWindow(QWidget *parent, const QString& sUserName)
{
    MainWindow* win = new MainWindow(parent);;
    if(win)
    {
        win->m_sUserName = sUserName;
        win->InitWindow();
        QRect screenRect = QGuiApplication::primaryScreen()->geometry();
        win->move((screenRect.width() - win->width()) / 2, (screenRect.height() - win->height()) / 2);//移动到所在屏幕中间
        win->show();
    }
    else
    {
        return false;
    }

    return true;
}
