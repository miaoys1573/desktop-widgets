#include "constants.h"
#include "textcheckbox.h"

void TextCheckBox::initUI()
{
    this->setCursor(QCursor(Qt::PointingHandCursor));
    this->setFocusPolicy(Qt::NoFocus);
}

void TextCheckBox::loadStyleSheet()
{
    QStringList styleSheet;
    styleSheet.append(QString("QCheckBox{background:%1;border-radius:%2px;height:%3px;}")
                      .arg(Constants::MAIN_BACKGROUND_1)
                      .arg(Constants::BUTTON_RADIUS)
                      .arg(Constants::CHECK_BUTTON_HEIGHT));
    styleSheet.append(QString("QCheckBox::indicator{background:%1;border-radius:%2px;width:%3px;height:%3px;}")
                      .arg(Constants::MAIN_BACKGROUND_1)
                      .arg(Constants::BUTTON_RADIUS)
                      .arg(Constants::CHECK_BUTTON_HEIGHT));
    styleSheet.append(QString("QCheckBox::indicator:checked{padding:%1px;width:%2px;height:%2px;image:url(:/assets/icons/check.svg)}")
                      .arg(Constants::BUTTON_PADDING)
                      .arg(Constants::CHECK_BUTTON_HEIGHT - 2 * Constants::BUTTON_PADDING));
    this->setStyleSheet(styleSheet.join(""));
}

TextCheckBox::TextCheckBox(QString text, QString objectName, QWidget *parent) : QCheckBox (parent)
{
    this->setText(text);
    this->setObjectName(objectName);
    this->initUI();
    this->loadStyleSheet();
}
