#include "closebutton.h"
#include "constants.h"

#include <qstyle.h>

void CloseButton::initUI()
{
    QStyle *style = this->style();
    QIcon icon = style->standardIcon(QStyle::SP_DialogCloseButton);
    this->setIcon(icon);
    this->setFixedSize(Constants::CLOSE_BUTTON_HEIGHT, Constants::CLOSE_BUTTON_HEIGHT);
}

void CloseButton::loadStyleSheet()
{
    QStringList styleSheet;
    styleSheet.append(QString("QPushButton{border-style:none;padding:%2px;border-radius:%3px;background:%4;}")
                      .arg(Constants::BUTTON_PADDING)
                      .arg(Constants::BUTTON_RADIUS)
                      .arg(Constants::MAIN_BACKGROUND_2));
    styleSheet.append(QString("QPushButton:hover{background:%2;}")
                      .arg(Constants::MAIN_BACKGROUND_3));
    this->setStyleSheet(styleSheet.join(""));
}

CloseButton::CloseButton(QWidget *parent) : BaseButton (parent)
{
    this->initUI();
    this->loadStyleSheet();
}
