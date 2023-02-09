#ifndef MAINSETTING_H
#define MAINSETTING_H

#include "basecard.h"
#include "themepanel.h"
#include "widgetspanel.h"
#include <QSystemTrayIcon>

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
    void loadStyleSheet();
    void initSignalSlots();

    QRadioButton *createMenuButton(QString name, QString objectName);

private slots:
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void toggleContentPanel();
    void backgroundTypeClick();
    void settingButtonClick();

public:
    MainSetting(QWidget *parent = 0);
    void showWidgets();
};

#endif // MAINSETTING_H
