#include "weather.h"

#include <base/baselabel.h>

void Weather::initUI()
{
    layout->addWidget(new BaseLabel("天气预报小部件", "weather-label", BaseLabel::TitleLabel));
}

void Weather::changeFontColor(QString color)
{
    QPalette palette;
    palette.setColor(QPalette::WindowText, color);
    this->findChild<BaseLabel*>("weather-label")->setPalette(palette);
    BaseCard::changeFontColor(color);
}

void Weather::updateData()
{

}

Weather::Weather(QWidget *parent) : BaseCard ("WEATHER", "天气预报", parent)
{
    this->initUI();
}
