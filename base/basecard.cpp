#include "basecard.h"
#include "constants.h"
#include "dbutil.h"
#include "iconbutton.h"
#include "slider.h"
#include <QFontDatabase>
#include <QSlider>

void BaseCard::initUI()
{
    // 默认不显示刷新按钮
    showRefreshButton = false;

    // 按钮组
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    IconButton *refreshButton = new IconButton("refresh");
    refreshButton->setVisible(showRefreshButton);
    buttonsLayout->addWidget(refreshButton, 0, Qt::AlignRight);
    buttonsLayout->addWidget(new IconButton("theme"), 0, Qt::AlignRight);
    buttonsLayout->addWidget(new IconButton("close"), 0, Qt::AlignRight);
    buttonsLayout->setAlignment(Qt::AlignRight);
    buttonsLayout->setContentsMargins(Constants::BUTTON_PADDING, Constants::BUTTON_PADDING, Constants::BUTTON_PADDING, 0);

    // 初始化主布局
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mainLayout);

    // 添加到主布局
    layout = new QVBoxLayout();
    layout->setContentsMargins(Constants::WIDGET_RADIUS, 0, Constants::WIDGET_RADIUS, Constants::WIDGET_RADIUS);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addLayout(layout);

    // 不显示任务栏图标
    this->setWindowFlag(Qt::Tool);
}

void BaseCard::initSignalSlots()
{
    connect(this->findChild<IconButton*>("refresh-button"), SIGNAL(clicked()), this, SLOT(refresh()));
    connect(this->findChild<IconButton*>("theme-button"), SIGNAL(clicked()), this, SLOT(showSettingPanel()));
    connect(this->findChild<IconButton*>("close-button"), SIGNAL(clicked()), this, SLOT(hide()));
    connect(this->findChild<IconButton*>("close-button"), SIGNAL(clicked()), _setting, SLOT(hide()));

    for (int i = 1; i <= _setting->getColors().size(); i++) {
        connect(_setting->findChild<QRadioButton*>(QString("background-pure-color-radio%1").arg(i)), SIGNAL(clicked()), this, SLOT(changeBackground()));
        connect(_setting->findChild<QRadioButton*>(QString("font-pure-color-radio%1").arg(i)), SIGNAL(clicked()), this, SLOT(changeFontColor()));
    }

    for (int i = 1; i <= _setting->getPresets().size(); i++) {
        connect(_setting->findChild<QRadioButton*>(QString("background-gradient-color-radio%1").arg(i)), SIGNAL(clicked()), this, SLOT(changeBackground()));
    }

    connect(_setting->findChild<Slider*>(), SIGNAL(valueChanged(int)), this, SLOT(changeBackgroundAlpha(int)));
}

void BaseCard::showEvent(QShowEvent *event)
{
    Setting setting = DbUtil::findSetting(name);

    if (setting.inited) {
        // 数据回显
        this->move(setting.positionX, setting.positionY);
        if ((int)BaseSetting::PureColor == setting.backgroundType) {
            _setting->findChild<ColorRadio*>("background-pure-color")->click();
            _setting->findChild<ColorRadio*>(QString("background-pure-color-radio%1").arg(setting.backgroundPureColorIndex + 1))->click();
            _setting->findChild<Slider*>()->setValue(setting.backgroundPureColorTransparence);
        } else if ((int)BaseSetting::GradientColor == setting.backgroundType) {
            _setting->findChild<ColorRadio*>("background-gradient-color")->click();
            _setting->findChild<ColorRadio*>(QString("background-gradient-color-radio%1").arg(setting.backgroundGradientColorIndex + 1))->click();
        }
        _setting->findChild<ColorRadio*>(QString("font-pure-color-radio%1").arg(setting.fontColorIndex + 1))->click();
    } else {
        // 初始化设置面板
        _setting->initSetting();
    }

    if (this->height() < Constants::WIDGET_HEIGHT) {
        this->setFixedHeight(Constants::WIDGET_HEIGHT);
    }

    // 数据更新
    this->updateData();

    BaseWidget::showEvent(event);
}

void BaseCard::closeEvent(QCloseEvent *event)
{
    if (_setting->isVisible()) {
        _setting->close();
    }
    if (timerId != -1) {
        this->killTimer(timerId);
    }
    BaseWidget::closeEvent(event);
}

void BaseCard::timerEvent(QTimerEvent *event)
{
    if (timerId != -1 && this->isVisible()) {
        this->updateData();
    }
    QObject::timerEvent(event);
}

void BaseCard::refresh()
{
    this->updateData();
}

void BaseCard::showSettingPanel()
{
    if (_setting->isVisible()) {
        _setting->activateWindow();
    } else {
        _setting->show();
    }
}

void BaseCard::changeBackground()
{
    if (_setting->backgroundType == BaseSetting::PureColor) {
        for (int i = 1; i <= _setting->getColors().size(); i++) {
            QRadioButton *colorRadio = _setting->findChild<QRadioButton*>(QString("background-pure-color-radio%1").arg(i));
            if (colorRadio->isChecked()) {
                QColor color = _setting->getColors().at(i - 1);
                backgroundPureColorIndex = i - 1;
                // 更新样式
                this->setBrush(QColor(color.red(), color.green(), color.blue(), backgroundPureColorAlpha));
                this->update();
                // 保存设置
                QStringList values = {QString::number((int)BaseSetting::PureColor), QString::number(i - 1)};
                DbUtil::updateSetting(name, {"background_type", "background_pure_color_index"}, values);
                break;
            }
        }
    } else {
        for (int i = 1; i <= _setting->getPresets().size(); i++) {
            QRadioButton *colorRadio = _setting->findChild<QRadioButton*>(QString("background-gradient-color-radio%1").arg(i));
            if (colorRadio->isChecked()) {
                QGradient::Preset preset = _setting->getPresets().at(i - 1);
                // 更新样式
                this->setBrush(QBrush(QGradient(preset)));
                this->update();
                // 保存设置
                QStringList values = {QString::number((int)BaseSetting::GradientColor), QString::number(i - 1)};
                DbUtil::updateSetting(name, {"background_type", "background_gradient_color_index"}, values);
                break;
            }
        }
    }
}

void BaseCard::changeBackgroundAlpha(int transparence)
{
    int opacity = 100 - transparence;
    int alpha = opacity * 255 / 100;
    backgroundPureColorAlpha = alpha;
    QColor backgroundPureColor = _setting->getColors().at(backgroundPureColorIndex);
    QColor color = QColor(backgroundPureColor.red(), backgroundPureColor.green(), backgroundPureColor.blue(), alpha);
    // 更新样式
    this->setBrush(QBrush(color));
    this->update();
    // 保存设置
    QStringList values = {QString::number(backgroundPureColorIndex), QString::number(transparence)};
    DbUtil::updateSetting(name, {"background_pure_color_index", "background_pure_color_transparence"}, values);
}

void BaseCard::changeFontColor()
{
    int size = _setting->getColors().size();
    for (int i = 1; i <= size; i++) {
        QRadioButton *fontColorRadio = _setting->findChild<QRadioButton*>(QString("font-pure-color-radio%1").arg(i));
        if (fontColorRadio->isChecked()) {
            QString color = _setting->getColors().at(i - 1);
            // 更新样式
            this->changeFontColor(color);
            // 保存设置
            DbUtil::updateSetting(name, "font_color_index", i - 1);
            break;
        }
    }
}

void BaseCard::setTimerInterval(int interval)
{
    timerId = this->startTimer(interval);
}

void BaseCard::changeFontColor(QString color)
{
    QList<IconButton*> iconButtons = this->findChildren<IconButton*>();
    foreach (IconButton* iconButton, iconButtons)
    {
        iconButton->setIconColor(color);
    }
}

void BaseCard::updateData()
{

}

void BaseCard::setShowRefreshButton(bool flag)
{
    showRefreshButton = flag;
    this->findChild<IconButton*>("refresh-button")->setVisible(true);
}

BaseCard::BaseCard(QString name, QString showName, QWidget *parent) : BaseWidget (parent)
{
    _setting = new BaseSetting;
    _setting->name = name;
    this->name = name;
    this->showName = showName;
    _setting->findChild<QLabel*>("title-label")->setText(showName);

    this->initUI();
    this->initSignalSlots();
}

BaseCard::BaseCard(QString name, QString showName, BaseSetting *setting, QWidget *parent) : BaseWidget (parent)
{
    if (setting == nullptr) {
        setting = new BaseSetting;
    }
   _setting = setting;
   _setting->name = name;
   this->name = name;
   this->showName = showName;
   _setting->findChild<QLabel*>("title-label")->setText(showName);

   this->initUI();
   this->initSignalSlots();
}

BaseSetting *BaseCard::getSetting()
{
    return _setting;
}
