#ifndef MAINSETTING_H
#define MAINSETTING_H

#include "themepanel.h"
#include "widgetspanel.h"
#include <QSystemTrayIcon>
#include <base/basewidget.h>

class MainSetting : public BaseWidget
{
    Q_OBJECT

private:
    QHBoxLayout *layout;
    ThemePanel *themePanel;
    WidgetsPanel *widgetsPanel;
    QSystemTrayIcon *systemTray;

    void initUI();
    void setAsidePanel();
    void setContentPanel();
    void loadSystemTray();
    void initSignalSlots();

private slots:
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void toggleContentPanel();
    void backgroundTypeClick();
    void themeButtonClick();

public:
    MainSetting(QWidget *parent = 0);
    void showWidgets();
};

#endif // MAINSETTING_H
