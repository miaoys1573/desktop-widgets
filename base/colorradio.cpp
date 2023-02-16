#include "colorradio.h"

#include <QPainter>
#include <QTime>

void ColorRadio::initUI()
{
    this->setCursor(QCursor(Qt::PointingHandCursor));
    this->setFocusPolicy(Qt::NoFocus);
    QString objectName = this->objectName();
    if (objectName == "") {
        objectName = "CR" + QTime::currentTime().toString("hhmmsszzz");
        this->setObjectName(objectName);
    }
    this->loadStyleSheet();
}

void ColorRadio::loadStyleSheet()
{
    QString objectName = this->objectName();
    QStringList styleSheet;
    styleSheet.append(QString("#%1::indicator{border-radius:5px;border:%2px solid #EFEFEF;width:%3px;height:%3px;}")
                      .arg(objectName).arg(_showBorder ? 3 : 0).arg(_showBorder ? 34 : 40));
    styleSheet.append(QString("#%1::indicator:checked{padding:%2px;width:26px;height:26px;image:url(:/assets/icons/check.svg)}")
                      .arg(objectName).arg(_showBorder ? 4 : 7));
    this->setStyleSheet(styleSheet.join(""));
}

void ColorRadio::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if (_activeColor) {
        painter.setBrush(QBrush(QColor(_background)));
    } else {
        painter.setBrush(QBrush(_gradient));
    }
    painter.setPen(Qt::transparent);
    QRect rect = this->rect();
    rect.setWidth(rect.width() - 6);
    rect.setHeight(rect.height());
    painter.drawRoundedRect(rect, 5, 5);
    QRadioButton::paintEvent(event);
}

ColorRadio::ColorRadio(QWidget *parent) : QRadioButton (parent)
{
    this->setBackground("#FFFFFF");
    this->initUI();
}

ColorRadio::ColorRadio(QGradient gradient, QWidget *parent) : QRadioButton (parent)
{
    this->setGradient(gradient);
    this->initUI();
}

ColorRadio::ColorRadio(QString background, QWidget *parent) : QRadioButton (parent)
{
    this->setBackground(background);
    this->initUI();
}

void ColorRadio::showBorder(bool flag)
{
    _showBorder = flag;
    this->loadStyleSheet();
}

void ColorRadio::showCheckFlag(bool flag)
{
    _showCheckFlag = flag;
    this->loadStyleSheet();
}

void ColorRadio::setObjName(QString name)
{
    QRadioButton::setObjectName(name);
    this->loadStyleSheet();
}

void ColorRadio::setGradient(QGradient gradient)
{
    _activeColor = false;
    _gradient = gradient;
    this->update();
}

void ColorRadio::setBackground(QString background)
{
    _activeColor = true;
    _background = background;
    this->update();
}
