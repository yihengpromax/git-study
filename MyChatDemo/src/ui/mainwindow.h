#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class NetworkManager;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void InitWindow();
    void InitConnect();

public:
    static bool ShowMainWindow(QWidget* parent, const QString& sUserName);

private slots:
    void OnBtnSendClicked();

private:
    Ui::MainWindow *ui;
    QString m_sUserName;
    NetworkManager *m_pNetWorkMgr;
};

#endif // MAINWINDOW_H
