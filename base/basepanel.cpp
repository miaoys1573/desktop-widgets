#include "basebutton.h"
#include "basepanel.h"
#include "closebutton.h"
#include "constants.h"

#include <QtEvents>
#include <qboxlayout.h>
#include <qframe.h>
#include <qpainter.h>
#include <qpainterpath.h>
#include <qstyle.h>

void BasePanel::initUI()
{
    this->setFixedSize(Constants::WIDGET_WIDTH, Constants::MAIN_SETTING_WIDGET_HEIGHT);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    // 内容区
    layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    QFrame *frame = new QFrame;
    frame->setObjectName("content-frame");
    frame->setLayout(layout);

    mainLayout->addWidget(new CloseButton, 0, Qt::AlignRight);
    mainLayout->addWidget(frame);
    this->setLayout(mainLayout);

    // 加载样式
    this->loadStyleSheet();
}

void BasePanel::loadStyleSheet()
{

}

void BasePanel::paintEvent(QPaintEvent *event)
{
    QPainterPath painterPath;
    painterPath.addRoundedRect(this->rect(), Constants::WIDGET_RADIUS, Constants::WIDGET_RADIUS);
    // 设置窗体左上角为直角
    painterPath.addRect(0, 0, width() / 2, height() / 2);
    // 设置窗体左下角为直角
    painterPath.addRect(0, height() / 2, width() / 2, height() / 2);
    painterPath.setFillRule(Qt::WindingFill);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // 和基础设置窗体背景色保持一致
    painter.setBrush(QBrush(QColor(Constants::MAIN_BACKGROUND_2)));
    painter.setPen(Qt::transparent);
    painter.drawPath(painterPath);

    QDialog::paintEvent(event);
}

void BasePanel::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Escape:
        break;
    default:
        QDialog::keyPressEvent(event);
    }
}

BasePanel::BasePanel(QWidget *parent) : QDialog(parent)
{
    this->initUI();
}
