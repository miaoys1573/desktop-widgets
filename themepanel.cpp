#include "themepanel.h"
#include <qpainter.h>
#include <qpainterpath.h>
#include <qslider.h>

void ThemePanel::initUI()
{
    // 基础设置窗口最大高度为551，该项设置用于禁止基础设置窗口高度自适应
    this->setFixedHeight(551);

    this->findChild<QLabel*>("theme-item-color")->hide();
    this->findChild<QFrame*>("background-pure-color-frame")->hide();
    this->findChild<QFrame*>("background-gradient-color-frame")->hide();
    this->findChild<QLabel*>("theme-item-transparence")->hide();
    this->findChild<QLabel*>("theme-item-transparence-value")->hide();
    this->findChild<QSlider*>("theme-item-slider")->hide();
}

void ThemePanel::initSignalSlots()
{

}

void ThemePanel::paintEvent(QPaintEvent *event)
{
    QPainterPath painterPath;
    painterPath.addRoundedRect(this->rect(), 15, 15);
    painterPath.addRect(0, 0, width() / 2, height() / 2);
    painterPath.addRect(0, height() / 2, width() / 2, height() / 2);
    painterPath.setFillRule(Qt::WindingFill);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(QColor(240, 240, 240, 250)));
    painter.setPen(Qt::transparent);
    painter.drawPath(painterPath);

    QDialog::paintEvent(event);
}

void ThemePanel::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
}

void ThemePanel::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
}

void ThemePanel::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
}

ThemePanel::ThemePanel(QWidget *parent) : BaseSetting (parent)
{
    this->initUI();
    this->initSignalSlots();
}
