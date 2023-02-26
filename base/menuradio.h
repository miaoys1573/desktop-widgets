#ifndef MENURADIO_H
#define MENURADIO_H

#include <QRadioButton>

class MenuRadio : public QRadioButton
{
    Q_OBJECT

private:
    void initUI();
    void loadStyleSheet();

public:
    MenuRadio(QString text, QString objectName, QWidget *parent = 0);
};

#endif // MENURADIO_H
