#ifndef WIDGETSPANEL_H
#define WIDGETSPANEL_H

#include <base/basecard.h>
#include <base/basepanel.h>
#include <base/textcheckbox.h>

class WidgetsPanel : public BasePanel
{
    Q_OBJECT

private:
    QList<BaseCard*> widgets;
    QMap<QString, TextCheckBox*> widgetItems;

    void initUI();
    void initSignalSlots();
    void initWidgets();

private slots:
    void toggleWidgetItem();

public:
    WidgetsPanel(QWidget *parent = 0);
    QList<BaseCard*> getWidgets();
    QMap<QString, TextCheckBox*> getWidgetItems();
};

#endif // WIDGETSPANEL_H
