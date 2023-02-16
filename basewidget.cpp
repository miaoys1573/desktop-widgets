#include "basewidget.h"
#include "dbutil.h"
#include <QPainter>
#include <QMouseEvent>

void BaseWidget::initUI()
{
    if (_height == -1) {
        this->setFixedWidth(_width);
    } else {
        this->setFixedSize(_width, _height);
    }
    // 不显示标题栏
    this->setWindowFlags(Qt::FramelessWindowHint);
    // 背景透明
    this->setAttribute(Qt::WA_TranslucentBackground);
}

void BaseWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    // 反锯齿
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(brush);
    // 设置边框透明
    painter.setPen(Qt::transparent);
    painter.drawRoundedRect(this->rect(), 15, 15);
    QDialog::paintEvent(event);
}

void BaseWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        QPoint startPos = event->globalPos();
        offPos = startPos - geometry().topLeft();
        this->setCursor(Qt::SizeAllCursor);
    }
    QDialog::mousePressEvent(event);
}

void BaseWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        QPoint endPos = event->globalPos();
        move(endPos - offPos);
    }
    QDialog::mouseMoveEvent(event);
}

void BaseWidget::mouseReleaseEvent(QMouseEvent *event)
{
    this->setCursor(Qt::ArrowCursor);
    QPoint pos = this->pos();
    DbUtil::updateSetting(name, {"position_x", "position_y"}, {QString::number(pos.x()), QString::number(pos.y())});
    QDialog::mouseReleaseEvent(event);
}

void BaseWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    // 取消Esc键关闭窗口
    case Qt::Key_Escape:
        break;
    default:
        QDialog::keyPressEvent(event);
    }
}

BaseWidget::BaseWidget(QWidget *parent)
    : QDialog(parent)
{
    this->_width = 400;
    this->_height = -1;
    this->brush = QBrush(QColor(255, 255, 255, 51));
    this->initUI();
}

BaseWidget::BaseWidget(QColor background, QWidget *parent)
    : QDialog(parent)
{
    this->_width = 400;
    this->_height = -1;
    this->brush = QBrush(background);
    this->initUI();
}

void BaseWidget::setBrush(QBrush brush)
{
    this->brush = brush;
}
