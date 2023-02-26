#ifndef ICONBUTTON_H
#define ICONBUTTON_H

#include "basebutton.h"

#include <QMap>

class IconButton : public BaseButton
{
    Q_OBJECT

private:
    QFont iconFont;
    QMap<QString, QChar> fontMap;

public:
    IconButton(QString iconName, QWidget *parent = 0);
    void setIconColor(QString iconColor);
};

#endif // ICONBUTTON_H
