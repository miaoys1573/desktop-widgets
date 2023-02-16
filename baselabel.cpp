#include "baselabel.h"

BaseLabel::BaseLabel(QWidget *parent)
    : QLabel (parent)
{

}

BaseLabel::BaseLabel(QString text, QString objectName, QWidget *parent)
    : QLabel (text, parent)
{
    this->setObjectName(objectName);
}

BaseLabel::BaseLabel(QString text, QString objectName, QString styleSheet, QWidget *parent)
    : QLabel (text, parent)
{
    this->setObjectName(objectName);
    this->setStyleSheet(styleSheet);
}

BaseLabel::BaseLabel(QString text, QString objectName, BaseLabel::type labeltype, QWidget *parent)
    : QLabel (text, parent)
{
    this->setObjectName(objectName);
    if (labeltype == BaseLabel::TitleLabel) {
        this->setStyleSheet("font-size:18px;font-weight:bold;padding:5px 0 5px 0;");
    }
}

BaseLabel::BaseLabel(QString text, QString objectName, BaseLabel::type labeltype, QString styleSheet, QWidget *parent)
    : QLabel (text, parent)
{
    this->setObjectName(objectName);
    if (labeltype == BaseLabel::TitleLabel) {
        this->setStyleSheet(QString("font-size:18px;font-weight:bold;%1;").arg(styleSheet));
    } else if (labeltype == BaseLabel::ContentLabel) {
        this->setStyleSheet(styleSheet);
    }
}
