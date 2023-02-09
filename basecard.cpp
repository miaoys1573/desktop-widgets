#include "basecard.h"
#include "dbutil.h"
#include "dbutil.h"

#include <QFontDatabase>
#include <QPushButton>
#include <QSlider>

void BaseCard::initUI()
{
    // 数据初始化
    showRefreshButton = false;
    int fontId = QFontDatabase::addApplicationFont(":/assets/fonts/iconfont.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    iconFont = QFont(fontFamily);
    fontMap.insert("refresh", QChar(0xe981));
    fontMap.insert("setting", QChar(0xe979));
    fontMap.insert("close", QChar(0xe980));
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mainLayout);

    // 按钮组
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    QPushButton *refreshButton = this->createButton("refresh");
    refreshButton->setVisible(showRefreshButton);
    buttonsLayout->addWidget(refreshButton, 0, Qt::AlignLeft);
    QPushButton *settingButton = this->createButton("setting");
    QPushButton *closeButton = this->createButton("close");
    buttonsLayout->addWidget(settingButton, 0, Qt::AlignRight);
    buttonsLayout->addWidget(closeButton, 0, Qt::AlignRight);
    buttonsLayout->setAlignment(Qt::AlignRight);
    buttonsLayout->setContentsMargins(8, 8, 8, 5);

    // 添加到主布局
    mainLayout->addLayout(buttonsLayout);
    layout = new QVBoxLayout();
    layout->setContentsMargins(15, 0, 15, 15);
    mainLayout->addLayout(layout);

    // 加载样式
    this->loadStyleSheet();

    // 不显示任务栏图标
    this->setWindowFlag(Qt::Tool);
}

void BaseCard::loadStyleSheet()
{
    QStringList styleSheet;
    styleSheet.append("#refresh-button,#setting-button,#close-button{width:15px;height:16px;padding:0;background:rgba(0,0,0,0);}");
    setStyleSheet(styleSheet.join(""));
}

void BaseCard::initSignalSlots()
{
    connect(this->findChild<QPushButton*>("refresh-button"), SIGNAL(clicked()), this, SLOT(refresh()));
    connect(this->findChild<QPushButton*>("setting-button"), SIGNAL(clicked()), this, SLOT(showSettingPanel()));
    connect(this->findChild<QPushButton*>("close-button"), SIGNAL(clicked()), this, SLOT(hide()));
    connect(this->findChild<QPushButton*>("close-button"), SIGNAL(clicked()),
            _setting, SLOT(hide()));

    for (int i = 1; i <= _setting->getColors().size(); i++) {
        connect(_setting->findChild<QRadioButton*>(QString("background-pure-color-radio%1").arg(i)), SIGNAL(clicked()), this, SLOT(changeBackground()));
        connect(_setting->findChild<QRadioButton*>(QString("font-pure-color-radio%1").arg(i)), SIGNAL(clicked()), this, SLOT(changeFontColor()));
    }
    for (int i = 1; i <= _setting->getPresets().size(); i++) {
        connect(_setting->findChild<QRadioButton*>(QString("background-gradient-color-radio%1").arg(i)), SIGNAL(clicked()), this, SLOT(changeBackground()));
    }
    connect(_setting->findChild<QSlider*>("theme-item-slider"), SIGNAL(valueChanged(int)), this, SLOT(changeBackgroundAlpha(int)));
}

QPushButton *BaseCard::createButton(QString name)
{
    QPushButton *button = new QPushButton();
    button->setObjectName(name + "-button");
    button->setCursor(QCursor(Qt::PointingHandCursor));
    button->setFocusPolicy(Qt::NoFocus);
    button->setFont(iconFont);
    button->setText(fontMap[name]);
    return button;
}

void BaseCard::showEvent(QShowEvent *event)
{
    Setting setting = DbUtil::findSetting(name);
    if (setting.inited) {
        this->move(setting.positionX, setting.positionY);
        if ((int)BaseSetting::PureColor == setting.backgroundType) {
            _setting->findChild<ColorRadio*>("background-pure-color")->click();
            _setting->findChild<ColorRadio*>(QString("background-pure-color-radio%1").arg(setting.backgroundPureColorIndex + 1))->click();
            _setting->findChild<QSlider*>("theme-item-slider")->setValue(setting.backgroundPureColorTransparence);
        } else if ((int)BaseSetting::GradientColor == setting.backgroundType) {
            _setting->findChild<ColorRadio*>("background-gradient-color")->click();
            _setting->findChild<ColorRadio*>(QString("background-gradient-color-radio%1").arg(setting.backgroundGradientColorIndex + 1))->click();
        }
        _setting->findChild<ColorRadio*>(QString("font-pure-color-radio%1").arg(setting.fontColorIndex + 1))->click();
    } else {
        _setting->initSetting();
    }
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
    if (timerId != -1) {
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
                this->setBrush(QColor(color.red(), color.green(), color.blue(), backgroundPureColorAlpha));
                this->update();
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
                this->setBrush(QBrush(QGradient(preset)));
                this->update();
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
    this->setBrush(QBrush(color));
    this->update();
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
            this->changeFontColor(color);
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
    if (showRefreshButton) {
        this->findChild<QPushButton*>("refresh-button")->setStyleSheet("color:" + color);
    }
    this->findChild<QPushButton*>("setting-button")->setStyleSheet("color:" + color);
    this->findChild<QPushButton*>("close-button")->setStyleSheet("color:" + color);
}

void BaseCard::updateData()
{

}

void BaseCard::setShowRefreshButton(bool flag)
{
    showRefreshButton = flag;
    this->findChild<QPushButton*>("refresh-button")->setVisible(true);
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
