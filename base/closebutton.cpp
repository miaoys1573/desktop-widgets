#include "closebutton.h"
#include "constants.h"

#include <qstyle.h>

void CloseButton::initUI()
{
    this->setFixedSize(Constants::CLOSE_BUTTON_HEIGHT, Constants::CLOSE_BUTTON_HEIGHT);
}

void CloseButton::loadStyleSheet()
{
    QStringList styleSheet;
    styleSheet.append(QString("QPushButton{border-style:none;border-radius:%1px;"
                              "background:%2;padding:3px;image:url(:/assets/icons/close.svg)}")
                      .arg(Constants::BUTTON_RADIUS)
                      .arg(Constants::MAIN_BACKGROUND_2));
    styleSheet.append(QString("QPushButton:hover{background:%1;}")
                      .arg(Constants::MAIN_BACKGROUND_3));
    this->setStyleSheet(styleSheet.join(""));
}

CloseButton::CloseButton(QWidget *parent) : BaseButton (parent)
{
    this->initUI();
    this->loadStyleSheet();
}
