#include "basebutton.h"

BaseButton::BaseButton(QString objectName, QWidget *parent) : QPushButton (parent)
{
    this->setObjectName(objectName);
    this->setCursor(QCursor(Qt::PointingHandCursor));
    this->setFocusPolicy(Qt::NoFocus);
}

BaseButton::BaseButton(QString text, QString objectName, QWidget *parent) : QPushButton (text, parent)
{
    this->setObjectName(objectName);
    this->setCursor(QCursor(Qt::PointingHandCursor));
    this->setFocusPolicy(Qt::NoFocus);
}
