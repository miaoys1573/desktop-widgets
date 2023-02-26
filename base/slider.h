#ifndef SLIDER_H
#define SLIDER_H

#include <qslider.h>

class Slider : public QSlider
{
    Q_OBJECT

private:
    void initUI();
    void loadStyleSheet();

public:
    Slider(QWidget *parent = 0);
};

#endif // SLIDER_H
