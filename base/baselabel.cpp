#include "baselabel.h"
#include "constants.h"

BaseLabel::BaseLabel(QWidget *parent)
    : QLabel (parent)
{

}

BaseLabel::BaseLabel(QString objectName, QWidget *parent)
    : QLabel (parent)
{
    this->setObjectName(objectName);
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
        this->setStyleSheet(QString("font-size:%1px;font-weight:bold;").arg(Constants::TITLE_FONT_SIZE));
    }
}

BaseLabel::BaseLabel(QString text, QString objectName, BaseLabel::type labeltype, QString styleSheet, QWidget *parent)
    : QLabel (text, parent)
{
    this->setObjectName(objectName);
    if (labeltype == BaseLabel::TitleLabel) {
        this->setStyleSheet(QString("font-size:%1px;font-weight:bold;%2;").arg(Constants::TITLE_FONT_SIZE).arg(styleSheet));
    } else if (labeltype == BaseLabel::ContentLabel) {
        this->setStyleSheet(styleSheet);
    }
}
