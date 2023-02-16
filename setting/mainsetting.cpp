#include "sysmonitor.h"
#include "mainsetting.h"
#include <QMenu>
#include <QRegularExpression>
#include <QStyle>
#include <qbuttongroup.h>
#include <qpushbutton.h>
#include <qslider.h>
#include <qtsingleapplication.h>

void MainSetting::initUI()
{
    this->setFixedWidth(580);
    // 主布局
    layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);
    // 左侧菜单栏
    this->setAsidePanel();
    // 内容区
    this->setContentPanel();
    // 系统托盘
    this->loadSystemTray();
    // 加载样式
    this->loadStyleSheet();
    // 不显示任务栏图标
    this->setWindowFlag(Qt::Tool);
}

void MainSetting::setAsidePanel()
{
    QVBoxLayout *asideLayout = new QVBoxLayout;
    asideLayout->setAlignment(Qt::AlignTop);
    asideLayout->setContentsMargins(10, 15, 5, 10);

    QRadioButton *widgetsMenu = this->createMenuButton("显示管理", "widgets-menu");
    QRadioButton *themeMenu = this->createMenuButton("全局样式", "theme-menu");
    QButtonGroup *menuGroup = new QButtonGroup;
    menuGroup->addButton(widgetsMenu);
    menuGroup->addButton(themeMenu);

    asideLayout->addWidget(widgetsMenu);
    asideLayout->addWidget(themeMenu);
    layout->addLayout(asideLayout);
}

void MainSetting::setContentPanel()
{
    themePanel->hide();
    layout->addWidget(themePanel);
    layout->addWidget(widgetsPanel);
}

void MainSetting::loadSystemTray()
{
    systemTray = new QSystemTrayIcon;
    QIcon icon(":/assets/icons/tray_icon.svg");
    systemTray->setIcon(icon);
    systemTray->setToolTip("桌面小部件Pro");

    // 系统托盘菜单
    QMenu *systemTrayMenu = new QMenu;
    QAction *quitAction = new QAction("退出");
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    systemTrayMenu->addAction(quitAction);

    systemTray->setContextMenu(systemTrayMenu);
    systemTray->show();
}

void MainSetting::loadStyleSheet()
{

}

void MainSetting::initSignalSlots()
{
    // 菜单
    connect(this->findChild<QRadioButton*>("widgets-menu"), SIGNAL(clicked()), this, SLOT(toggleContentPanel()));
    connect(this->findChild<QRadioButton*>("theme-menu"), SIGNAL(clicked()), this, SLOT(toggleContentPanel()));

    connect(widgetsPanel->findChild<QPushButton*>("close-button"), SIGNAL(clicked()), this, SLOT(close()));
    // 主题面板
    connect(themePanel->findChild<QPushButton*>("close-button"), SIGNAL(clicked()), this, SLOT(close()));
    QList<BaseCard*> widgets = widgetsPanel->getWidgets();
    foreach (BaseCard *widget, widgets) {
        connect(widget->findChild<QPushButton*>("setting-button"), SIGNAL(clicked()), this, SLOT(settingButtonClick()));
        connect(widget->getSetting()->findChild<QPushButton*>("close-button"), SIGNAL(clicked()), this, SLOT(settingButtonClick()));
        connect(widget->findChild<QPushButton*>("close-button"), SIGNAL(clicked()), this, SLOT(settingButtonClick()));
        // 背景类型
        connect(themePanel->findChild<ColorRadio*>("background-pure-color"), SIGNAL(clicked()),
                widget->getSetting()->findChild<ColorRadio*>("background-pure-color"), SLOT(click()));
        connect(themePanel->findChild<ColorRadio*>("background-pure-color"), SIGNAL(clicked()),
                widget->getSetting()->findChild<ColorRadio*>("background-pure-color-radio1"), SLOT(click()));
        connect(themePanel->findChild<ColorRadio*>("background-gradient-color"), SIGNAL(clicked()),
                widget->getSetting()->findChild<ColorRadio*>("background-gradient-color"), SLOT(click()));
        connect(themePanel->findChild<ColorRadio*>("background-gradient-color"), SIGNAL(clicked()),
                widget->getSetting()->findChild<ColorRadio*>("background-gradient-color-radio1"), SLOT(click()));
        // 纯色背景 + 字体
        for (int i = 1; i <= themePanel->getColors().size(); i++) {
            connect(themePanel->findChild<QRadioButton*>(QString("background-pure-color-radio%1").arg(i)), SIGNAL(clicked()),
                    widget->getSetting()->findChild<QRadioButton*>(QString("background-pure-color-radio%1").arg(i)), SLOT(click()));
            connect(themePanel->findChild<QRadioButton*>(QString("font-pure-color-radio%1").arg(i)), SIGNAL(clicked()),
                    widget->getSetting()->findChild<QRadioButton*>(QString("font-pure-color-radio%1").arg(i)), SLOT(click()));
        }
        // 渐变色背景
        for (int i = 1; i <= themePanel->getPresets().size(); i++) {
            connect(themePanel->findChild<QRadioButton*>(QString("background-gradient-color-radio%1").arg(i)), SIGNAL(clicked()),
                    widget->getSetting()->findChild<QRadioButton*>(QString("background-gradient-color-radio%1").arg(i)), SLOT(click()));
        }
        // 纯色背景透明度
        connect(themePanel->findChild<QSlider*>("theme-item-slider"), SIGNAL(valueChanged(int)),
                widget->getSetting()->findChild<QSlider*>("theme-item-slider"), SLOT(setValue(int)));
        // 重置按钮
        connect(themePanel->findChild<QPushButton*>("reset-button"), SIGNAL(clicked()),
                widget->getSetting()->findChild<QPushButton*>("reset-button"), SLOT(click()));
    }
    connect(themePanel->findChild<ColorRadio*>("background-pure-color"), SIGNAL(clicked()),
            this, SLOT(backgroundTypeClick()));

    // 系统托盘
    connect(systemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
}

QRadioButton *MainSetting::createMenuButton(QString name, QString objectName)
{
    QRadioButton *menuButton = new QRadioButton(" " + name);
    menuButton->setObjectName(objectName);
    menuButton->setCursor(QCursor(Qt::PointingHandCursor));
    menuButton->setFocusPolicy(Qt::NoFocus);
    menuButton->setIcon(QIcon(QString(":/assets/icons/%1.svg").arg(objectName.split("-")[0])));
    menuButton->setIconSize(QSize(20, 20));
    QStringList styleSheet;
    styleSheet.append(QString("#%1{background:#FFFFFF;border-radius:5px;height:40px;}").arg(objectName));
    styleSheet.append(QString("#%1:hover{background:#EFEFEF;}").arg(objectName));
    styleSheet.append(QString("#%1:checked{background:#EFEFEF;}").arg(objectName));
    styleSheet.append(QString("#%1::indicator{border-style:none;}").arg(objectName));
    menuButton->setStyleSheet(styleSheet.join(""));
    return menuButton;
}

void MainSetting::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (QSystemTrayIcon::Trigger == reason) {
        if (this->isHidden()) {
            this->findChild<QRadioButton*>("widgets-menu")->click();
            this->show();
        } else if (this->isVisible()) {
            this->hide();
        }
    }
}

void MainSetting::toggleContentPanel()
{
    QList<QRadioButton*> menus;
    menus.append(this->findChild<QRadioButton*>("widgets-menu"));
    menus.append(this->findChild<QRadioButton*>("theme-menu"));
    foreach (QRadioButton* menu, menus) {
        if (menu->isChecked()) {
            this->findChild<QWidget*>(menu->objectName().split("-")[0] + "-panel")->show();
        } else {
            this->findChild<QWidget*>(menu->objectName().split("-")[0] + "-panel")->hide();
        }
    }
}

void MainSetting::backgroundTypeClick()
{
    themePanel->findChild<QSlider*>("theme-item-slider")->setValue(80);
}

void MainSetting::settingButtonClick()
{
    QList<BaseCard*> widgets = widgetsPanel->getWidgets();
    int count = 0;
    foreach (BaseCard *widget, widgets) {
        if (widget->getSetting()->isVisible()) {
            count++;
        }
    }
    foreach (BaseCard *widget, widgets) {
        if (count == 0) {
            widget->findChild<QPushButton*>("setting-button")->show();
        } else {
            if (widget->getSetting()->isVisible()) {
                widget->findChild<QPushButton*>("setting-button")->show();
            } else {
                widget->findChild<QPushButton*>("setting-button")->hide();
            }
        }
    }
}

MainSetting::MainSetting(QWidget *parent) : BaseWidget (QColor(255, 255, 255), parent)
{
    widgetsPanel = new WidgetsPanel;
    widgetsPanel->setObjectName("widgets-panel");
    themePanel = new ThemePanel;
    themePanel->setObjectName("theme-panel");

    this->initUI();
    this->initSignalSlots();

    // 显示主窗口
    QMap<QString, QCheckBox*> widgetItems = widgetsPanel->getWidgetItems();
    QList<QCheckBox*> widgetStatusList = widgetItems.values();
    int count = 0;
    foreach (QCheckBox* widgetStatus, widgetStatusList) {
        if (widgetStatus->isChecked()) {
            count++;
        }
    }
    if (count == 0) {
        this->show();
    }
}

void MainSetting::showWidgets()
{
    QList<BaseCard*> widgets = widgetsPanel->getWidgets();
    QMap<QString, QCheckBox*> widgetItems = widgetsPanel->getWidgetItems();
    foreach (BaseCard *widget, widgets) {
        if (widgetItems[widget->name]->isChecked()) {
            widget->show();
        }
    }
}
