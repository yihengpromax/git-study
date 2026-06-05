#ifndef LOADINGBUBBLEDIALOG_H
#define LOADINGBUBBLEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QTimer>
#include <QPropertyAnimation>

// 自定义旋转加载控件 (环形渐变Spinner)
class SpinnerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SpinnerWidget(QWidget *parent = nullptr);
    void startSpinning();
    void stopSpinning();
    QSize sizeHint() const override { return QSize(40, 40); }

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void rotateStep();

private:
    QTimer   m_rotationTimer;
    int      m_angle;          // 当前旋转角度 (0~360)
    QColor   m_colorPrimary;
    QColor   m_colorSecondary;
};

// 高级加载气泡弹窗 (带阴影、圆角、淡入淡出)
class LoadingBubbleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadingBubbleDialog(const QString &text = tr("正在加载..."), QWidget *parent = nullptr);
    ~LoadingBubbleDialog();

    void setText(const QString &text);
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    bool IsStarted();

public:
    static LoadingBubbleDialog* GetInstance(const QString &text = tr("正在加载..."), QWidget *parent = nullptr);

public slots:
    void startLoading();      // 显示并开始动画
    void stopLoading();       // 结束动画并关闭窗口

private:
    void centerOnParent();    // 居中于父窗口或屏幕

public:
    static LoadingBubbleDialog *m_pInstance;

private:
    SpinnerWidget *m_spinner;
    QLabel        *m_textLabel;
    QWidget       *m_bubble;          // 气泡容器（带圆角和阴影）
    bool          m_bStarted;
};

#endif // LOADINGBUBBLEDIALOG_H