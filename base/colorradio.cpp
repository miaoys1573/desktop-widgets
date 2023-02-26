#include "colorradio.h"
#include "constants.h"

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
    int height = Constants::CHECK_BUTTON_HEIGHT;
    int borderWidth = Constants::CHECK_BUTTON_BORDER_WIDTH;
    int padding = Constants::BUTTON_PADDING;
    QStringList styleSheet;
    styleSheet.append(QString("QRadioButton::indicator{border-radius:%1px;border:%2px solid %3;width:%4px;height:%4px;}")
                      .arg(Constants::BUTTON_RADIUS)
                      .arg(_showBorder ? borderWidth : 0)
                      .arg(Constants::MAIN_BACKGROUND_2)
                      .arg(_showBorder ? height - 2 * borderWidth : height));
    styleSheet.append(QString("QRadioButton::indicator:checked{padding:%1px;width:%2px;height:%2px;image:url(:/assets/icons/check.svg)}")
                      .arg(_showBorder ? padding - borderWidth : padding)
                      .arg(height - 2 * padding));
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
    painter.drawRoundedRect(rect, Constants::BUTTON_RADIUS, Constants::BUTTON_RADIUS);
    QRadioButton::paintEvent(event);
}

ColorRadio::ColorRadio(QWidget *parent) : QRadioButton (parent)
{
    this->setBackground(Constants::MAIN_BACKGROUND_1);
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
