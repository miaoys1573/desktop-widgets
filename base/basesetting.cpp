#include "basesetting.h"
#include "baselabel.h"
#include "basebutton.h"
#include "dbutil.h"
#include "constants.h"
#include "closebutton.h"
#include "slider.h"
#include <QButtonGroup>
#include <QFile>
#include <QMetaEnum>
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
    QGridLayout *titleBarLayout = new QGridLayout;
    titleBarLayout->setContentsMargins(0, 0, 0, 0);
    titleBarLayout->addWidget(new QLabel, 0, 0);
    titleBarLayout->addWidget(new BaseLabel("", "title-label", BaseLabel::TitleLabel, "padding:0;"), 0, 1, Qt::AlignCenter);
    titleBarLayout->addWidget(new CloseButton, 0, 2, Qt::AlignRight);

    // 添加到主布局
    layout->addLayout(titleBarLayout);
}

void BaseSetting::setThemePanel()
{
    // 内容
    QString settingItemTitleMarginTop = QString("margin-top:%1px").arg(Constants::SETTING_ITEM_TITLE_MARGIN_TOP);

    QVBoxLayout *themeLayout = new QVBoxLayout;
    themeLayout->addWidget(new BaseLabel("背景", "theme-item-background"));
    themeLayout->addLayout(this->createBackgroundTypeLayout());
    themeLayout->addWidget(new BaseLabel("颜色", "theme-item-color", settingItemTitleMarginTop));
    themeLayout->addWidget(this->createPureColorPanel("background"));
    themeLayout->addWidget(this->createGradientColorPanel("background"));

    QHBoxLayout *themeItemLayout = new QHBoxLayout;
    themeItemLayout->addWidget(new BaseLabel("透明度", "theme-item-transparence", settingItemTitleMarginTop), 0, Qt::AlignLeft);
    themeItemLayout->addWidget(new BaseLabel("0%", "theme-item-transparence-value", settingItemTitleMarginTop), 0, Qt::AlignRight);
    themeLayout->addLayout(themeItemLayout);

    themeLayout->addWidget(new Slider);
    themeLayout->addWidget(new BaseLabel("字体", "theme-item-font", settingItemTitleMarginTop));
    themeLayout->addWidget(this->createPureColorPanel("font"));

    QFrame *themeFrame = new QFrame();
    themeFrame->setObjectName("theme-frame");
    themeFrame->setLayout(themeLayout);

    // 添加到主布局
    QString settingItemMarginTop = QString("margin:%1px 0").arg(Constants::SETTING_ITEM_MARGIN_TOP / 4);
    layout->addWidget(new BaseLabel("主题", "theme-label", BaseLabel::TitleLabel, settingItemMarginTop));
    layout->addWidget(themeFrame);
}

void BaseSetting::setResetPanel()
{
    // 内容
    QFrame *resetFrame = new QFrame();
    resetFrame->setObjectName("reset-frame");
    BaseButton *resetButton = new BaseButton("恢复默认", "reset-button");

    QVBoxLayout *resetLayout = new QVBoxLayout;
    resetLayout->addWidget(resetButton);
    resetFrame->setLayout(resetLayout);

    // 添加到主布局
    QString marginTop = QString("margin:%1px 0 %2px 0").arg(Constants::SETTING_ITEM_MARGIN_TOP).arg(Constants::SETTING_ITEM_MARGIN_TOP / 4);
    layout->addWidget(new BaseLabel("重置", "reset-label", BaseLabel::TitleLabel, marginTop));
    layout->addWidget(resetFrame);
    layout->addWidget(new BaseLabel);
}

void BaseSetting::loadStyleSheet()
{
    QStringList settingStyle;
    // 设置项
    QString contentStyle = QString("{background:%1;border-radius:%2px;padding:%3px;}")
            .arg(Constants::MAIN_BACKGROUND_1).arg(Constants::SETTING_ITEM_RADIUS).arg("%1");
    // 主题
    settingStyle.append("#theme-frame" + contentStyle.arg(Constants::SETTING_ITEM_PADDING));
    // 重置
    settingStyle.append("#reset-frame" + contentStyle.arg(0));
    settingStyle.append(QString("#reset-button{border-style:none;border-radius:%1px;padding:%2px;background:%3;color:%4;}")
                        .arg(Constants::BUTTON_RADIUS)
                        .arg(Constants::BUTTON_PADDING)
                        .arg(Constants::MAIN_BACKGROUND_1)
                        .arg(Constants::RESET_BUTTON_COLOR));
    settingStyle.append(QString("#reset-button:hover{background:%1;}")
                        .arg(Constants::RESET_BUTTON_HOVER_BACKGROUND));
    setStyleSheet(settingStyle.join(""));
}

void BaseSetting::initSignalSlots()
{
    connect(this->findChild<Slider*>(), SIGNAL(valueChanged(int)), this, SLOT(sliderChange(int)));
    connect(this->findChild<CloseButton*>(), SIGNAL(clicked()), this, SLOT(hide()));
    connect(this->findChild<BaseButton*>("reset-button"), SIGNAL(clicked()), this, SLOT(reset()));
    connect(this->findChild<ColorRadio*>("background-pure-color"), SIGNAL(clicked()), this, SLOT(toggleBackgroundColorPanel()));
    connect(this->findChild<ColorRadio*>("background-gradient-color"), SIGNAL(clicked()), this, SLOT(toggleBackgroundColorPanel()));
}

ColorRadio *BaseSetting::createColorRadio(QString objName, QString background)
{
    ColorRadio *colorRadio = new ColorRadio(background);
    colorRadio->setObjectName(objName);
    return colorRadio;
}

ColorRadio *BaseSetting::createColorRadio(QString objName, QGradient::Preset preset)
{
    ColorRadio *colorRadio = new ColorRadio(preset);
    colorRadio->setObjectName(objName);
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
    ColorRadio *pureColor = new ColorRadio(Constants::MAIN_BACKGROUND_1);
    pureColor->showBorder(true);
    pureColor->setObjectName("background-pure-color");
    ColorRadio *gradientColor = new ColorRadio(QGradient::WarmFlame);
    gradientColor->setObjectName("background-gradient-color");

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
        this->findChild<Slider*>()->show();
        this->findChild<QRadioButton*>(QString("background-pure-color-radio%1")
                .arg(setting.inited ? setting.backgroundPureColorIndex + 1 : 1))->click();
        int transparence = Constants::WIDGET_TRANSPARENCE;
        if (setting.inited && setting.backgroundPureColorTransparence > 0) {
            transparence = setting.backgroundPureColorTransparence;
        }
        if (!name.isEmpty()) {
            this->findChild<Slider*>()->setValue(transparence);
        }
        this->resize(size);
    } else if (this->findChild<ColorRadio*>("background-gradient-color")->isChecked()) {
        this->backgroundType = BaseSetting::GradientColor;
        this->findChild<QFrame*>("background-pure-color-frame")->hide();
        this->findChild<QLabel*>("theme-item-transparence")->hide();
        this->findChild<QLabel*>("theme-item-transparence-value")->hide();
        this->findChild<Slider*>()->hide();
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
    this->findChild<Slider*>()->setValue(Constants::WIDGET_TRANSPARENCE);
    this->findChild<QRadioButton*>("font-pure-color-radio1")->click();
}

BaseSetting::BaseSetting(QWidget *parent) : BaseWidget(Constants::MAIN_BACKGROUND_2, parent)
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
