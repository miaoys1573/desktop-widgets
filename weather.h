#ifndef WEATHER_H
#define WEATHER_H

#include "basecard.h"

class Weather : public BaseCard
{
    Q_OBJECT

private:
    void initUI();
    void changeFontColor(QString color) Q_DECL_OVERRIDE;
    void updateData() Q_DECL_OVERRIDE;

public:
    Weather(QWidget *parent = 0);
};

#endif // WEATHER_H
