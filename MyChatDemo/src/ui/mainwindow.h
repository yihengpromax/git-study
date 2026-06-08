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

signals:
    void SendChat(const QString &toUsername, const QString &content);
    void SendGetFriendList();
    void SendAddFriend(const QString &username, const QString &friendName);

private slots:
    void OnBtnSendClicked();
    void OnGetFriendList(const QJsonArray &friends);
    void OnAddFriendResult(bool ok, const QString& err);
    void OnBtnAddFriendClicked();

private:
    Ui::MainWindow *ui;
    QString m_sUserName;
};

#endif // MAINWINDOW_H
