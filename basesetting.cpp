#include "basesetting.h"
#include "baselabel.h"
#include "dbutil.h"
#include "basebutton.h"
#include <QButtonGroup>
#include <QFile>
#include <QMetaEnum>
#include <QScrollArea>
#include <QSizePolicy>
#include <QSlider>
#include <QStyle>

void BaseSetting::initUI()
{
    // 标题栏
    this->setTitleBar();
    // 主题
    this->setThemePanel();
    // 重置
    this->setResetPanel();
    // 加载样式
    this->loadStyleSheet();
    // 不显示任务栏图标
    this->setWindowFlag(Qt::Tool);
}

void BaseSetting::setTitleBar()
{
    // 内容
    BaseLabel *title = new BaseLabel("", "title-label", BaseLabel::TitleLabel, "padding:0;");
    QStyle *style = this->style();
    QIcon icon = style->standardIcon(QStyle::SP_DialogCloseButton);
    BaseButton *closeButton = new BaseButton("close-button");
    closeButton->setIcon(icon);

    // 布局
    QGridLayout *titleBarLayout = new QGridLayout;
    titleBarLayout->setContentsMargins(0, 0, 0, 0);
    titleBarLayout->addWidget(new QLabel, 0, 0);
    titleBarLayout->addWidget(title, 0, 1, Qt::AlignCenter);
    titleBarLayout->addWidget(closeButton, 0, 2, Qt::AlignRight);

    // 添加到主布局
    layout->addLayout(titleBarLayout);
}

void BaseSetting::setThemePanel()
{
    // 内容
    QFrame *themeFrame = new QFrame();
    themeFrame->setObjectName("theme-frame");
    QSlider *themeItemSlider = new QSlider;
    themeItemSlider->setObjectName("theme-item-slider");
    themeItemSlider->setRange(0, 100);
    themeItemSlider->setOrientation(Qt::Horizontal);
    themeItemSlider->setCursor(QCursor(Qt::PointingHandCursor));

    // 布局
    QString paddingTop = "padding-top:10px";
    QVBoxLayout *themeLayout = new QVBoxLayout;
    QHBoxLayout *themeItemLayout = new QHBoxLayout;
    themeItemLayout->addWidget(new BaseLabel("透明度", "theme-item-transparence", BaseLabel::ContentLabel, paddingTop), 0, Qt::AlignLeft);
    themeItemLayout->addWidget(new BaseLabel("0%", "theme-item-transparence-value", BaseLabel::ContentLabel, paddingTop), 0, Qt::AlignRight);
    themeLayout->addWidget(new BaseLabel("背景", "theme-item-background", BaseLabel::ContentLabel));
    themeLayout->addLayout(this->createBackgroundTypeLayout());
    themeLayout->addWidget(new BaseLabel("颜色", "theme-item-color", BaseLabel::ContentLabel, paddingTop));
    themeLayout->addWidget(this->createPureColorPanel("background"));
    themeLayout->addWidget(this->createGradientColorPanel("background"));
    themeLayout->addLayout(themeItemLayout);
    themeLayout->addWidget(themeItemSlider);
    themeLayout->addWidget(new BaseLabel("字体", "theme-item-font", BaseLabel::ContentLabel, paddingTop));
    themeLayout->addWidget(this->createPureColorPanel("font"));
    themeFrame->setLayout(themeLayout);

    // 添加到主布局
    layout->addWidget(new BaseLabel("主题", "theme-label", BaseLabel::TitleLabel));
    layout->addWidget(themeFrame);
}

void BaseSetting::setResetPanel()
{
    // 内容
    QFrame *resetFrame = new QFrame();
    resetFrame->setObjectName("reset-frame");
    BaseButton *resetButton = new BaseButton("恢复默认", "reset-button");

    // 布局
    QVBoxLayout *resetLayout = new QVBoxLayout;
    resetLayout->addWidget(resetButton);
    resetFrame->setLayout(resetLayout);

    // 添加到主布局
    layout->addWidget(new BaseLabel("重置", "reset-label", BaseLabel::TitleLabel, "padding-top:20px;"));
    layout->addWidget(resetFrame);
    layout->addWidget(new BaseLabel);
}

void BaseSetting::loadStyleSheet()
{
    QStringList settingStyle;
    // 设置项
    QString contentStyle = "{background:#FFFFFF;border-radius:10px;padding:%1;}";

    // 标题栏
    settingStyle.append("#close-button{border-style:none;padding:8px;border-radius:5px;background:#EFEFEF;}");
    settingStyle.append("#close-button:hover{background:#E4E4E4;}");

    // 主题
    settingStyle.append("#theme-frame" + contentStyle.arg("5px"));
    int sliderHeight = 10;
    QString normalColor = "#ECECEC";
    QString grooveColor = "#1ABC9C";
    QString handleBorderColor = "#1ABC9C";
    QString handleColor = "#FFFFFF";
    int sliderRadius = sliderHeight / 2;
    int handleWidth = (sliderHeight * 3) / 2 + (sliderHeight / 5) + 1;
    int handleRadius = handleWidth / 2;
    int handleOffset = handleRadius / 2;
    settingStyle.append(QString("QSlider::horizontal{min-height:%1px;}").arg(sliderHeight * 2));
    settingStyle.append(QString("QSlider::groove:horizontal{background:%1;height:%2px;border-radius:%3px;}")
                       .arg(normalColor).arg(sliderHeight).arg(sliderRadius));
    settingStyle.append(QString("QSlider::add-page:horizontal{background:%1;height:%2px;border-radius:%3px;}")
                       .arg(normalColor).arg(sliderHeight).arg(sliderRadius));
    settingStyle.append(QString("QSlider::sub-page:horizontal{background:%1;height:%2px;border-radius:%3px;}")
                       .arg(grooveColor).arg(sliderHeight).arg(sliderRadius));
    settingStyle.append(QString("QSlider::handle:horizontal{width:%3px;margin-top:-%4px;margin-bottom:-%4px;border-radius:%5px;"
                        "background:qradialgradient(spread:pad,cx:0.5,cy:0.5,radius:0.5,fx:0.5,fy:0.5,stop:0.6 %1,stop:0.8 %2);}")
                       .arg(handleColor).arg(handleBorderColor).arg(handleWidth).arg(handleOffset).arg(handleRadius));

    // 重置
    settingStyle.append("#reset-frame" + contentStyle.arg(0));
    settingStyle.append("#reset-button{border-style:none;border-radius:5px;padding:5px;background:#FFFFFF;color:rgb(255,90,93);}");
    settingStyle.append("#reset-button:pressed{background:rgb(251,251,251);}");

    setStyleSheet(settingStyle.join(""));
}

void BaseSetting::initSignalSlots()
{
    connect(this->findChild<QSlider*>("theme-item-slider"), SIGNAL(valueChanged(int)), this, SLOT(sliderChange(int)));
    connect(this->findChild<BaseButton*>("close-button"), SIGNAL(clicked()), this, SLOT(hide()));
    connect(this->findChild<BaseButton*>("reset-button"), SIGNAL(clicked()), this, SLOT(reset()));
    connect(this->findChild<ColorRadio*>("background-pure-color"), SIGNAL(clicked()), this, SLOT(toggleBackgroundColorPanel()));
    connect(this->findChild<ColorRadio*>("background-gradient-color"), SIGNAL(clicked()), this, SLOT(toggleBackgroundColorPanel()));
}

ColorRadio *BaseSetting::createColorRadio(QString objName, QString background)
{
    ColorRadio *colorRadio = new ColorRadio(background);
    colorRadio->setObjName(objName);
    return colorRadio;
}

ColorRadio *BaseSetting::createColorRadio(QString objName, QGradient::Preset preset)
{
    ColorRadio *colorRadio = new ColorRadio(preset);
    colorRadio->setObjName(objName);
    return colorRadio;
}

QFrame *BaseSetting::createPureColorPanel(QString name)
{
    QFrame *frame = new QFrame;
    frame->setObjectName(name + "-pure-color-frame");
    QHBoxLayout *layout = new QHBoxLayout;
    QButtonGroup *radioGroup = new QButtonGroup;
    for (int i = 1; i <= colors.size(); i++) {
        QString objName = QString(name + "-pure-color-radio%1").arg(i);
        ColorRadio *colorRadio = this->createColorRadio(objName, colors.at(i - 1));
        if (i == 1) {
            colorRadio->showBorder(true);
        }
        radioGroup->addButton(colorRadio);
        layout->addWidget(colorRadio);
    }
    layout->setContentsMargins(0, 0, 0, 0);
    frame->setLayout(layout);
    return frame;
}

QFrame *BaseSetting::createGradientColorPanel(QString name)
{
    QFrame *frame = new QFrame;
    frame->setObjectName(name + "-gradient-color-frame");
    QGridLayout *layout = new QGridLayout();
    QButtonGroup *radioGroup = new QButtonGroup;
    for (int i = 1; i <= presets.size(); i++) {
        QString objName = QString(name + "-gradient-color-radio%1").arg(i);
        ColorRadio *colorRadio = this->createColorRadio(objName, presets.at(i - 1));
        radioGroup->addButton(colorRadio);
        layout->addWidget(colorRadio, (i - 1) / 7, (i - 1) % 7, 1, 1);
    }
    layout->setContentsMargins(0, 0, 0, 0);
    frame->setLayout(layout);
    return frame;
}

QLayout *BaseSetting::createBackgroundTypeLayout()
{
    ColorRadio *pureColor = new ColorRadio("#FFFFFF");
    pureColor->showBorder(true);
    pureColor->setObjName("background-pure-color");
    ColorRadio *gradientColor = new ColorRadio(QGradient::WarmFlame);
    gradientColor->setObjName("background-gradient-color");

    QButtonGroup *radioGroup = new QButtonGroup;
    radioGroup->addButton(pureColor);
    radioGroup->addButton(gradientColor);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setAlignment(Qt::AlignLeft);
    layout->addWidget(pureColor);
    layout->addWidget(gradientColor);
    return layout;
}

void BaseSetting::toggleBackgroundColorPanel()
{
    Setting setting = DbUtil::findSetting(name);
    this->findChild<QLabel*>("theme-item-color")->show();
    if (this->findChild<ColorRadio*>("background-pure-color")->isChecked()) {
        this->backgroundType = BaseSetting::PureColor;
        this->findChild<QFrame*>("background-gradient-color-frame")->hide();
        this->findChild<QFrame*>("background-pure-color-frame")->show();
        this->findChild<QLabel*>("theme-item-transparence")->show();
        this->findChild<QLabel*>("theme-item-transparence-value")->show();
        this->findChild<QSlider*>("theme-item-slider")->show();
        this->findChild<QRadioButton*>(QString("background-pure-color-radio%1")
                .arg(setting.inited ? setting.backgroundPureColorIndex + 1 : 1))->click();
        int transparence = 80;
        if (setting.inited && setting.backgroundPureColorTransparence > 0) {
            transparence = setting.backgroundPureColorTransparence;
        }
        if (!name.isEmpty()) {
            this->findChild<QSlider*>("theme-item-slider")->setValue(transparence);
        }
        this->resize(size);
    } else if (this->findChild<ColorRadio*>("background-gradient-color")->isChecked()) {
        this->backgroundType = BaseSetting::GradientColor;
        this->findChild<QFrame*>("background-pure-color-frame")->hide();
        this->findChild<QLabel*>("theme-item-transparence")->hide();
        this->findChild<QLabel*>("theme-item-transparence-value")->hide();
        this->findChild<QSlider*>("theme-item-slider")->hide();
        this->findChild<QFrame*>("background-gradient-color-frame")->show();
        this->findChild<QRadioButton*>(QString("background-gradient-color-radio%1")
                .arg(setting.inited ? setting.backgroundGradientColorIndex + 1 : 1))->click();
    }
}

void BaseSetting::sliderChange(int value)
{
    this->findChild<QLabel*>("theme-item-transparence-value")->setText(QString("%1%").arg(value));
}

void BaseSetting::reset()
{
    this->findChild<ColorRadio*>("background-pure-color")->click();
    this->findChild<QRadioButton*>("background-pure-color-radio1")->click();
    this->findChild<QSlider*>("theme-item-slider")->setValue(80);
    this->findChild<QRadioButton*>("font-pure-color-radio1")->click();
}

BaseSetting::BaseSetting(QWidget *parent) : BaseWidget(QColor(240, 240, 240, 250), parent)
{
    // 颜色块
    colors = QString("#FFFFFF,#1E90FF,#00CED1,#90EE90,#FF8C00,#FF4500,#4D4D4D").split(",");
    // 渐变主题
    presets = {QGradient::WarmFlame, QGradient::SunnyMorning, QGradient::WinterNeva, QGradient::HeavyRain, QGradient::AmyCrisp,
               QGradient::DeepBlue, QGradient::RareWind, QGradient::SaintPetersburg, QGradient::StrongBliss, QGradient::SoftGrass,
               QGradient::DirtyBeauty, QGradient::GreatWhale, QGradient::CochitiLake, QGradient::MountainRock, QGradient::DesertHump,
               QGradient::JungleDay, QGradient::ConfidentCloud, QGradient::MarbleWall, QGradient::MoleHall, QGradient::SandStrike,
               QGradient::StrictNovember};
    // 主布局
    layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignTop);
    this->setLayout(layout);

    this->initUI();
    this->initSignalSlots();
    size = QSize(this->width(), this->height());
}

QStringList BaseSetting::getColors()
{
    return colors;
}

QVector<QGradient::Preset> BaseSetting::getPresets()
{
    return presets;
}

void BaseSetting::initSetting()
{
    this->reset();
}
