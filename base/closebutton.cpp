#include "closebutton.h"
#include "constants.h"

#include <qstyle.h>

void CloseButton::initUI()
{
    this->setFixedSize(Constants::CLOSE_BUTTON_HEIGHT, Constants::CLOSE_BUTTON_HEIGHT);
}

void CloseButton::loadStyleSheet()
{
    int padding = Constants::CLOSE_BUTTON_HEIGHT * Constants::BUTTON_PADDING / Constants::CHECK_BUTTON_HEIGHT;
    QStringList styleSheet;
    styleSheet.append(QString("QPushButton{border-style:none;border-radius:%1px;"
                              "background:%2;padding:3%;image:url(:/assets/icons/close.svg)}")
                      .arg(Constants::BUTTON_RADIUS)
                      .arg(Constants::MAIN_BACKGROUND_2)
                      .arg(padding));
    styleSheet.append(QString("QPushButton:hover{background:%2;}")
                      .arg(Constants::MAIN_BACKGROUND_3));
    this->setStyleSheet(styleSheet.join(""));
}

CloseButton::CloseButton(QWidget *parent) : BaseButton (parent)
{
    this->initUI();
    this->loadStyleSheet();
}
