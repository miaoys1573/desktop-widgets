#ifndef WIDGETSPANEL_H
#define WIDGETSPANEL_H

#include "basecard.h"
#include "basepanel.h"
#include <qcheckbox.h>

class WidgetsPanel : public BasePanel
{
    Q_OBJECT

private:
    QList<BaseCard*> widgets;
    QMap<QString, QCheckBox*> widgetItems;

    void initUI();
    void loadStyleSheet();
    void initSignalSlots();

    void initWidgets();
    QCheckBox* createWidgetItem(QString name, QString objectName);

private slots:
    void toggleWidgetItem();

public:
    WidgetsPanel(QWidget *parent = 0);
    QList<BaseCard*> getWidgets();
    QMap<QString, QCheckBox*> getWidgetItems();
};

#endif // WIDGETSPANEL_H
