#ifndef LOGINFRAME_H
#define LOGINFRAME_H

#include <QMainWindow>

namespace Ui {
class LoginFrame;
}

class LoginFrame : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginFrame(QWidget *parent = nullptr);
    ~LoginFrame();

    static LoginFrame* ShowLoginFrame(QWidget *parent = nullptr);
    void InitWindow();
    void InitSignalWithSlot();

private slots:
    void OnBtnLoginClicked();
    void OnBtnRegisterClicked();
    void OnBtnModifyPassword();

private:
    Ui::LoginFrame *ui;
};

#endif // LOGINFRAME_H
