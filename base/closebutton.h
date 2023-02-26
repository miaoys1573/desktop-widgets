#ifndef CLOSEBUTTON_H
#define CLOSEBUTTON_H

#include "basebutton.h"

class CloseButton : public BaseButton
{
    Q_OBJECT

private:
    void initUI();
    void loadStyleSheet();

public:
    CloseButton(QWidget *parent = 0);
};

#endif // CLOSEBUTTON_H
