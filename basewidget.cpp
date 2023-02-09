#include "basewidget.h"
#include "dbutil.h"
#include <QPainter>
#include <QMouseEvent>

void BaseWidget::_initUI()
{
    if (_height == -1) {
        this->setFixedWidth(_width);
    } else {
        this->setFixedSize(_width, _height);
    }
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

void BaseWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(_brush);
    painter.setPen(Qt::transparent);
    painter.drawRoundedRect(this->rect(), 15, 15);
    QDialog::paintEvent(event);
}

void BaseWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        QPoint startPos = event->globalPos();
        _offPos = startPos - geometry().topLeft();
        this->setCursor(Qt::SizeAllCursor);
    }
    QDialog::mousePressEvent(event);
}

void BaseWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        QPoint endPos = event->globalPos();
        move(endPos - _offPos);
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
    case Qt::Key_Escape:
        break;
    default:
        QDialog::keyPressEvent(event);
    }
}

BaseWidget::BaseWidget(QWidget *parent)
    : QDialog(parent)
{
    _width = 400;
    _height = -1;
    _brush = QBrush(QColor(255, 255, 255, 51));
    this->_initUI();
}

BaseWidget::BaseWidget(QColor background, QWidget *parent)
    : QDialog(parent)
{
    _width = 400;
    _height = -1;
    _brush = QBrush(background);
    this->_initUI();
}

BaseWidget::BaseWidget(int width, int height, QColor background, QWidget *parent)
    : QDialog(parent)
{
    _width = width;
    _height = height;
    _brush = QBrush(background);
    this->_initUI();
}

void BaseWidget::setBrush(QBrush brush)
{
    _brush = brush;
}
