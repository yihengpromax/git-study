#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void InitWindow();

public:
    static bool ShowMainWindow(QWidget* parent, const QString& sUserName);

private:
    Ui::MainWindow *ui;
    QString m_sUserName;
};

#endif // MAINWINDOW_H
