#ifndef BASESETTING_H
#define BASESETTING_H

#include "colorradio.h"

#include <basewidget.h>
#include <qboxlayout.h>
#include <qlabel.h>
#include <qradiobutton.h>

class BaseSetting : public BaseWidget
{
    Q_OBJECT

private:
    // 纯色色系
    QStringList colors;
    // 渐变色色系
    QVector<QGradient::Preset> presets;
    QVBoxLayout *layout;
    // 用于窗体高度自适应
    QSize size;

    void initUI();
    void setTitleBar();
    void setThemePanel();
    void setResetPanel();
    void loadStyleSheet();
    void initSignalSlots();

    ColorRadio *createColorRadio(QString objName, QString background);
    ColorRadio *createColorRadio(QString objName, QGradient::Preset preset);
    QFrame *createPureColorPanel(QString name);
    QFrame *createGradientColorPanel(QString name);
    QLayout *createBackgroundTypeLayout();

private slots:
    void toggleBackgroundColorPanel();
    void sliderChange(int value);
    void reset();

public:
    enum BackgroundType {
        PureColor = 1,
        GradientColor = 2
    };
    Q_ENUM(BackgroundType);

    QString name;
    BackgroundType backgroundType;

    BaseSetting(QWidget *parent = 0);
    QStringList getColors();
    QVector<QGradient::Preset> getPresets();
    void initSetting();
};

#endif // BASESETTING_H
