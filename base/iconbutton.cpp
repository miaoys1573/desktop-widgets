#include "constants.h"
#include "iconbutton.h"

#include <qfontdatabase.h>

IconButton::IconButton(QString iconName, QWidget *parent) : BaseButton (parent)
{
    int fontId = QFontDatabase::addApplicationFont(":/assets/fonts/iconfont.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    iconFont = QFont(fontFamily);
    iconFont.setPixelSize(Constants::MAJOR_FONT_SIZE);

    fontMap.insert("refresh", QChar(0xe902));
    fontMap.insert("theme", QChar(0xe900));
    fontMap.insert("close", QChar(0xe901));

    this->setFont(iconFont);
    this->setText(fontMap[iconName]);
    this->setObjectName(iconName + "-button");

    this->setIconColor(Constants::MAIN_BACKGROUND_1);
}

void IconButton::setIconColor(QString iconColor)
{
    QColor color(iconColor);
    int gradient = 30;
    QString hoverColor = QString("rgb(%1,%2,%3)")
            .arg(color.red() < gradient ? color.red() : color.red() - gradient)
            .arg(color.green() < gradient ? color.green() : color.green() - gradient)
            .arg(color.blue() < gradient ? color.blue() : color.blue() - gradient);
    QStringList styleSheet;
    styleSheet.append(QString("QPushButton{width:15px;height:16px;padding:0;background:rgba(0,0,0,0);color:%1}").arg(iconColor));
    styleSheet.append(QString("QPushButton:hover{color:%1}").arg(hoverColor));
    this->setStyleSheet(styleSheet.join(""));
}
