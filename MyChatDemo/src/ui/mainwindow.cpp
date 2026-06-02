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

}

bool MainWindow::ShowMainWindow(QWidget *parent)
{
    MainWindow* win = new MainWindow(parent);;
    if(win)
    {
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
