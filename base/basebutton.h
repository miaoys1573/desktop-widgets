#ifndef BASEBUTTON_H
#define BASEBUTTON_H

#include <qpushbutton.h>

class BaseButton : public QPushButton
{
    Q_OBJECT

public:
    BaseButton(QString objectName, QWidget *parent = 0);
    BaseButton(QString text, QString objectName, QWidget *parent = 0);
};

#endif // BASEBUTTON_H
