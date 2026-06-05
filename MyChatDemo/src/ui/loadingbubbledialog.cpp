#include "loadingbubbledialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>
#include <QScreen>
#include <QApplication>

// ========== SpinnerWidget 实现 ==========
SpinnerWidget::SpinnerWidget(QWidget *parent)
    : QWidget(parent), m_angle(0), m_colorPrimary(52, 152, 219), m_colorSecondary(155, 200, 240)
{
    setFixedSize(40, 40);
    setAttribute(Qt::WA_TranslucentBackground);
    connect(&m_rotationTimer, &QTimer::timeout, this, &SpinnerWidget::rotateStep);
}

void SpinnerWidget::startSpinning()
{
    if (!m_rotationTimer.isActive()) {
        m_angle = 0;
        m_rotationTimer.start(30); // 约33fps，平滑旋转
    }
}

void SpinnerWidget::stopSpinning()
{
    m_rotationTimer.stop();
}

void SpinnerWidget::rotateStep()
{
    m_angle += 10;
    if (m_angle >= 360) m_angle -= 360;
    update();
}

void SpinnerWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int side = qMin(width(), height());
    QRectF rect(2, 2, side-4, side-4);
    int penWidth = 3;
    painter.setPen(QPen(m_colorPrimary, penWidth, Qt::SolidLine, Qt::RoundCap));

    // 绘制未完成的背景圆弧 (浅色)
    painter.setPen(QPen(m_colorSecondary, penWidth, Qt::SolidLine, Qt::RoundCap));
    painter.drawArc(rect, 0, 360 * 16);

    // 绘制前景旋转圆弧 (动态角度)
    painter.setPen(QPen(m_colorPrimary, penWidth, Qt::SolidLine, Qt::RoundCap));
    int spanAngle = 120 * 16;  // 120度弧长
    int startAngle = (m_angle * 16);
    painter.drawArc(rect, startAngle, spanAngle);
}



// ========== LoadingBubbleDialog 实现 ==========
LoadingBubbleDialog *LoadingBubbleDialog::m_pInstance = nullptr;
LoadingBubbleDialog::LoadingBubbleDialog(const QString &text, QWidget *parent)
    : QDialog(parent)
    , m_bStarted(false)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);          // 模态，阻塞父窗口交互
    setFocusPolicy(Qt::NoFocus);

    // 创建气泡容器（圆角卡片）
    m_bubble = new QWidget(this);
    m_bubble->setObjectName("bubble");
    m_bubble->setStyleSheet(
        "QWidget#bubble {"
        "   background-color: rgba(30, 30, 35, 220);"   // 深色半透明背景，现代感
        "   border-radius: 24px;"
        "}"
        );

    // 添加阴影效果
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(m_bubble);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 80));
    shadow->setOffset(0, 6);
    m_bubble->setGraphicsEffect(shadow);

    // 创建旋转控件和文本
    m_spinner = new SpinnerWidget(m_bubble);
    m_textLabel = new QLabel(text, m_bubble);
    m_textLabel->setStyleSheet(
        "QLabel {"
        "   color: white;"
        "   font-size: 15px;"
        "   font-family: 'Segoe UI', 'Microsoft YaHei';"
        "   font-weight: normal;"
        "   background: transparent;"
        "}"
        );
    m_textLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // 水平布局：图标 + 文字
    QHBoxLayout *bubbleLayout = new QHBoxLayout(m_bubble);
    bubbleLayout->setContentsMargins(20, 14, 24, 14);
    bubbleLayout->setSpacing(12);
    bubbleLayout->addWidget(m_spinner);
    bubbleLayout->addWidget(m_textLabel);

    // 主布局（使气泡居中）
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_bubble, 0, Qt::AlignCenter);

    resize(220, 80);  // 弹窗默认大小（足够容纳内容）
}

LoadingBubbleDialog::~LoadingBubbleDialog()
{
    stopLoading();
}

void LoadingBubbleDialog::setText(const QString &text)
{
    m_textLabel->setText(text);
}

void LoadingBubbleDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    centerOnParent();
    // m_fadeInAnim->start();
    m_spinner->startSpinning();
}

void LoadingBubbleDialog::hideEvent(QHideEvent *event)
{
    m_spinner->stopSpinning();
    QDialog::hideEvent(event);
}

bool LoadingBubbleDialog::IsStarted()
{
    return m_bStarted;
}

LoadingBubbleDialog* LoadingBubbleDialog::GetInstance(const QString &text, QWidget *parent)
{
    if (!m_pInstance)
    {
        m_pInstance = new LoadingBubbleDialog(text, parent);
    }

    return m_pInstance;
}

void LoadingBubbleDialog::startLoading()
{
    m_bStarted = true;
    if (isVisible())
    {
        m_spinner->startSpinning();
        raise();
    }
    else
    {
        show();
    }
}

void LoadingBubbleDialog::stopLoading()
{
    m_bStarted = false;
    hide();
}

void LoadingBubbleDialog::centerOnParent()
{
    if (parentWidget())
    {
        // 相对父窗口居中
        QPoint center = parentWidget()->geometry().center();
        move(center.x() - width() / 2, center.y() - height() / 2);
    }
    else
    {
        // 无父窗口时屏幕居中
        QRect screenGeometry = QApplication::primaryScreen()->geometry();
        move(screenGeometry.center() - rect().center());
    }
}