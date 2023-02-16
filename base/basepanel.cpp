#include "basepanel.h"

#include <QtEvents>
#include <qboxlayout.h>
#include <qframe.h>
#include <qpainter.h>
#include <qpainterpath.h>
#include <qpushbutton.h>
#include <qstyle.h>

void BasePanel::initUI()
{
    // 基础设置窗体宽度为400，最大高度为551
    this->setFixedSize(400, 551);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    // 关闭按钮
    QStyle *style = this->style();
    QIcon icon = style->standardIcon(QStyle::SP_DialogCloseButton);
    QPushButton *closeButton = new QPushButton();
    closeButton->setObjectName("close-button");
    closeButton->setIcon(icon);
    closeButton->setCursor(QCursor(Qt::PointingHandCursor));
    closeButton->setFocusPolicy(Qt::NoFocus);

    // 内容区
    layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    QFrame *frame = new QFrame;
    frame->setObjectName("content-frame");
    frame->setLayout(layout);

    mainLayout->addWidget(closeButton, 0, Qt::AlignRight);
    mainLayout->addWidget(frame);
    this->setLayout(mainLayout);

    // 加载样式
    this->loadStyleSheet();
}

void BasePanel::loadStyleSheet()
{
    QStringList styleSheet;
    styleSheet.append("#close-button{border-style:none;padding:8px;border-radius:5px;background:#EFEFEF;}");
    styleSheet.append("#close-button:hover{background:#E4E4E4;}");
    setStyleSheet(styleSheet.join(""));
}

void BasePanel::paintEvent(QPaintEvent *event)
{
    QPainterPath painterPath;
    painterPath.addRoundedRect(this->rect(), 15, 15);
    // 设置窗体左上角为直角
    painterPath.addRect(0, 0, width() / 2, height() / 2);
    // 设置窗体左下角为直角
    painterPath.addRect(0, height() / 2, width() / 2, height() / 2);
    painterPath.setFillRule(Qt::WindingFill);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // 和基础设置窗体背景色保持一致
    painter.setBrush(QBrush(QColor(240, 240, 240, 250)));
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
