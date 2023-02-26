#ifndef TEXTCHECKBOX_H
#define TEXTCHECKBOX_H

#include <qcheckbox.h>

class TextCheckBox : public QCheckBox
{
    Q_OBJECT

private:
    void initUI();
    void loadStyleSheet();

public:
    TextCheckBox(QString text, QString objectName, QWidget *parent = 0);
};

#endif // TEXTCHECKBOX_H
