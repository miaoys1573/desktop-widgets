#include "constants.h"
#include "slider.h"

void Slider::initUI()
{
    this->setRange(0, 100);
    this->setOrientation(Qt::Horizontal);
    this->setCursor(QCursor(Qt::PointingHandCursor));
}

void Slider::loadStyleSheet()
{
    QString objectName = this->objectName();
    QStringList styleSheet;
    int sliderHeight = Constants::SLIDER_HEIGHT;
    QString background = Constants::MAIN_BACKGROUND_2;
    int sliderRadius = sliderHeight / 2;
    int handleWidth = (sliderHeight * 3) / 2 + (sliderHeight / 5) + 1;
    int handleRadius = handleWidth / 2;
    int handleOffset = handleRadius / 2;
    styleSheet.append(QString("QSlider::horizontal{min-height:%1px;}").arg(sliderHeight * 2));
    styleSheet.append(QString("QSlider::groove:horizontal{background:%1;height:%2px;border-radius:%3px;}")
                      .arg(background).arg(sliderHeight).arg(sliderRadius));
    styleSheet.append(QString("QSlider::add-page:horizontal{background:%1;height:%2px;border-radius:%3px;}")
                      .arg(background).arg(sliderHeight).arg(sliderRadius));
    styleSheet.append(QString("QSlider::sub-page:horizontal{background:%1;height:%2px;border-radius:%3px;}")
                      .arg(Constants::MAIN_COLOR).arg(sliderHeight).arg(sliderRadius));
    styleSheet.append(QString("QSlider::handle:horizontal{width:%3px;margin-top:-%4px;margin-bottom:-%4px;border-radius:%5px;"
                       "background:qradialgradient(spread:pad,cx:0.5,cy:0.5,radius:0.5,fx:0.5,fy:0.5,stop:0.6 %1,stop:0.8 %2);}")
                      .arg(Constants::MAIN_BACKGROUND_1).arg(Constants::MAIN_COLOR).arg(handleWidth)
                      .arg(handleOffset).arg(handleRadius));
    this->setStyleSheet(styleSheet.join(""));
}

Slider::Slider(QWidget *parent) : QSlider (parent)
{
    this->initUI();
    this->loadStyleSheet();
}
