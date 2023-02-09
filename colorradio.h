#ifndef COLORRADIO_H
#define COLORRADIO_H

#include <qradiobutton.h>

class ColorRadio : public QRadioButton
{

private:
    bool _showBorder = false;
    bool _showCheckFlag = true;
    bool _activeColor = true;
    QGradient _gradient;
    QString _background;

    void initUI();
    void loadStyleSheet();
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

public:
    ColorRadio(QWidget *parent = 0);
    ColorRadio(QGradient gradient, QWidget *parent = 0);
    ColorRadio(QString background, QWidget *parent = 0);
    void showBorder(bool flag);
    void showCheckFlag(bool flag);
    void setObjName(QString name);
    void setGradient(QGradient gradient);
    void setBackground(QString background);
};

#endif // COLORRADIO_H
