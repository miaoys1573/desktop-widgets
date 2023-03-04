#include "baselabel.h"
#include "constants.h"

void BaseLabel::setFontSize(int size)
{
    QFont font = this->font();
    font.setPixelSize(size);
    this->setFont(font);
}

BaseLabel::BaseLabel(QWidget *parent)
    : QLabel (parent)
{

}

BaseLabel::BaseLabel(QString objectName, QWidget *parent)
    : QLabel (parent)
{
    this->setObjectName(objectName);
    this->setFontSize(Constants::MAJOR_FONT_SIZE);
}

BaseLabel::BaseLabel(QString text, QString objectName, QWidget *parent)
    : QLabel (text, parent)
{
    this->setObjectName(objectName);
    this->setFontSize(Constants::MAJOR_FONT_SIZE);
}

BaseLabel::BaseLabel(QString text, QString objectName, QString styleSheet, QWidget *parent)
    : QLabel (text, parent)
{
    this->setObjectName(objectName);
    this->setFontSize(Constants::MAJOR_FONT_SIZE);
    this->setStyleSheet(styleSheet);
}

BaseLabel::BaseLabel(QString text, QString objectName, BaseLabel::type labelType, QWidget *parent)
    : QLabel (text, parent)
{
    this->setObjectName(objectName);
    if (labelType == BaseLabel::TitleLabel) {
        this->setStyleSheet("font-weight:500");
        this->setFontSize(Constants::TITLE_FONT_SIZE);
    } else {
        this->setFontSize(Constants::MAJOR_FONT_SIZE);
    }
}

BaseLabel::BaseLabel(QString text, QString objectName, BaseLabel::type labelType, QString styleSheet, QWidget *parent)
    : QLabel (text, parent)
{
    this->setObjectName(objectName);
    if (labelType == BaseLabel::TitleLabel) {
        this->setStyleSheet(QString("font-weight:500;%1;").arg(styleSheet));
        this->setFontSize(Constants::TITLE_FONT_SIZE);
    } else {
        this->setStyleSheet(styleSheet);
        this->setFontSize(Constants::MAJOR_FONT_SIZE);
    }
}
