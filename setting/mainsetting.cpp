#include "mainsetting.h"
#include <QMenu>
#include <QStyle>
#include <qbuttongroup.h>
#include <qslider.h>
#include <qtsingleapplication.h>
#include <base/closebutton.h>
#include <base/constants.h>
#include <base/iconbutton.h>
#include <base/menuradio.h>
#include <base/slider.h>

void MainSetting::initUI()
{
    this->setFixedWidth(Constants::MAIN_SETTING_WIDGET_WIDTH);
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
    // 不显示任务栏图标
    this->setWindowFlag(Qt::Tool);
}

void MainSetting::setAsidePanel()
{
    QVBoxLayout *asideLayout = new QVBoxLayout;
    asideLayout->setAlignment(Qt::AlignTop);
    int radius = Constants::WIDGET_RADIUS;
    asideLayout->setContentsMargins(radius, radius, radius - 5, radius);

    MenuRadio *widgetsMenu = new MenuRadio("显示管理", "widgets-menu");
    MenuRadio *themeMenu = new MenuRadio("全局样式", "theme-menu");
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
    systemTray->setToolTip(Constants::SYSTEM_TRAY_TOOLTIP);

    // 系统托盘菜单
    QMenu *systemTrayMenu = new QMenu;
    QAction *quitAction = new QAction("退出");
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    systemTrayMenu->addAction(quitAction);

    systemTray->setContextMenu(systemTrayMenu);
    systemTray->show();
}

void MainSetting::initSignalSlots()
{
    // 菜单
    connect(this->findChild<MenuRadio*>("widgets-menu"), SIGNAL(clicked()), this, SLOT(toggleContentPanel()));
    connect(this->findChild<MenuRadio*>("theme-menu"), SIGNAL(clicked()), this, SLOT(toggleContentPanel()));

    connect(widgetsPanel->findChild<CloseButton*>(), SIGNAL(clicked()), this, SLOT(close()));

    // 主题面板
    connect(themePanel->findChild<CloseButton*>(), SIGNAL(clicked()), this, SLOT(close()));
    QList<BaseCard*> widgets = widgetsPanel->getWidgets();
    foreach (BaseCard *widget, widgets) {

        connect(widget->findChild<IconButton*>("theme-button"), SIGNAL(clicked()), this, SLOT(themeButtonClick()));
        connect(widget->getSetting()->findChild<CloseButton*>(), SIGNAL(clicked()), this, SLOT(themeButtonClick()));
        connect(widget->findChild<IconButton*>("close-button"), SIGNAL(clicked()), this, SLOT(themeButtonClick()));

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
        connect(themePanel->findChild<Slider*>(), SIGNAL(valueChanged(int)),
                widget->getSetting()->findChild<Slider*>(), SLOT(setValue(int)));

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

void MainSetting::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (QSystemTrayIcon::Trigger == reason) {
        if (this->isHidden()) {
            this->findChild<MenuRadio*>("widgets-menu")->click();
            this->show();
        } else if (this->isVisible()) {
            this->hide();
        }
    }
}

void MainSetting::toggleContentPanel()
{
    QList<MenuRadio*> menus;
    menus.append(this->findChild<MenuRadio*>("widgets-menu"));
    menus.append(this->findChild<MenuRadio*>("theme-menu"));
    foreach (MenuRadio* menu, menus) {
        if (menu->isChecked()) {
            this->findChild<QWidget*>(menu->objectName().split("-")[0] + "-panel")->show();
        } else {
            this->findChild<QWidget*>(menu->objectName().split("-")[0] + "-panel")->hide();
        }
    }
}

void MainSetting::backgroundTypeClick()
{
    themePanel->findChild<Slider*>()->setValue(Constants::WIDGET_TRANSPARENCE);
}

void MainSetting::themeButtonClick()
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
            widget->findChild<IconButton*>("theme-button")->show();
        } else {
            if (widget->getSetting()->isVisible()) {
                widget->findChild<IconButton*>("theme-button")->show();
            } else {
                widget->findChild<IconButton*>("theme-button")->hide();
            }
        }
    }
}

MainSetting::MainSetting(QWidget *parent) : BaseWidget (QColor(Constants::MAIN_BACKGROUND_1), parent)
{
    widgetsPanel = new WidgetsPanel;
    widgetsPanel->setObjectName("widgets-panel");
    themePanel = new ThemePanel;
    themePanel->setObjectName("theme-panel");

    this->initUI();
    this->initSignalSlots();

    // 显示主窗口
    QMap<QString, TextCheckBox*> widgetItems = widgetsPanel->getWidgetItems();
    QList<TextCheckBox*> widgetStatusList = widgetItems.values();
    int count = 0;
    foreach (TextCheckBox* widgetStatus, widgetStatusList) {
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
    QMap<QString, TextCheckBox*> widgetItems = widgetsPanel->getWidgetItems();
    foreach (BaseCard *widget, widgets) {
        if (widgetItems[widget->name]->isChecked()) {
            widget->show();
        }
    }
}
