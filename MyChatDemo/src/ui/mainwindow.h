#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

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
    void InitChatInfo();

public:
    static bool ShowMainWindow(QWidget* parent, const QString& sUserName);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

signals:
    void SendChatReq(const QString &toUsername, const QString &content);
    void SendGetFriendListReq();
    void SendAddFriendReq(const QString &username, const QString &friendName);
    void SendGetOfflineMsgReq(const QString &username);

private slots:
    void OnBtnSendClicked();
    void OnGetFriendList(const QJsonArray &friends);
    void OnAddFriendResult(bool ok, const QString& err);
    void OnBtnAddFriendClicked();
    void OnChatMessageReceived(int fromId, const QString &fromUserName, const QString &content, const QString &datetime);

private:
    void appendBubble(const QString &text, bool isSelf);

private:
    Ui::MainWindow *ui;
    QString m_sUserName;
};

#endif // MAINWINDOW_H
