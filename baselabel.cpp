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

BaseLabel::BaseLabel(QString text, QString objectName, BaseLabel::type labeltype, QString padding, QWidget *parent)
    : QLabel (text, parent)
{
    this->setObjectName(objectName);
    if (labeltype == BaseLabel::TitleLabel) {
        this->setStyleSheet(QString("font-size:18px;font-weight:bold;padding:%1;").arg(padding));
    } else if (labeltype == BaseLabel::ContentLabel) {
        this->setStyleSheet(QString("padding:%1;").arg(padding));
    }
}

BaseLabel::BaseLabel(QString text, QString objectName, BaseLabel::type labeltype, int paddingTop, QWidget *parent)
    : QLabel (text, parent)
{
    this->setObjectName(objectName);
    if (labeltype == BaseLabel::TitleLabel) {
        this->setStyleSheet(QString("font-size:18px;font-weight:bold;padding:%1px 0 5px 0;").arg(paddingTop));
    } else if (labeltype == BaseLabel::ContentLabel) {
        this->setStyleSheet(QString("padding-top:%1px;").arg(paddingTop));
    }
}
