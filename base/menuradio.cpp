#include "constants.h"
#include "menuradio.h"

void MenuRadio::initUI()
{
    this->setCursor(QCursor(Qt::PointingHandCursor));
    this->setFocusPolicy(Qt::NoFocus);
    this->setIcon(QIcon(QString(":/assets/icons/%1.svg").arg(this->objectName().split("-")[0])));
    this->setIconSize(QSize(Constants::CHECK_BUTTON_HEIGHT / 2, Constants::CHECK_BUTTON_HEIGHT / 2));
}

void MenuRadio::loadStyleSheet()
{
    QStringList styleSheet;
    styleSheet.append(QString("QRadioButton{background:%1;border-radius:%2px;height:%3px;}")
                      .arg(Constants::MAIN_BACKGROUND_1)
                      .arg(Constants::BUTTON_RADIUS)
                      .arg(Constants::CHECK_BUTTON_HEIGHT));
    styleSheet.append(QString("QRadioButton:hover{background:%1;}")
                      .arg(Constants::MAIN_BACKGROUND_2));
    styleSheet.append(QString("QRadioButton:checked{background:%1;}")
                      .arg(Constants::MAIN_BACKGROUND_2));
    styleSheet.append(QString("QRadioButton::indicator{border-style:none;}"));
    this->setStyleSheet(styleSheet.join(""));
}

MenuRadio::MenuRadio(QString text, QString objectName, QWidget *parent) : QRadioButton (parent)
{
    this->setText(" " + text);
    this->setObjectName(objectName);
    this->initUI();
    this->loadStyleSheet();
}
