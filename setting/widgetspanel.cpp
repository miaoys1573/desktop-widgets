#include "widgetspanel.h"

#include <base/dbutil.h>
#include <base/iconbutton.h>
#include <widgets/movie.h>
#include <widgets/news.h>
#include <widgets/sysmonitor.h>
#include <widgets/yiwen.h>
#include <widgets/yiyan.h>

void WidgetsPanel::initUI()
{
    QGridLayout *contentlayout = new QGridLayout;
    contentlayout->setAlignment(Qt::AlignTop);
    contentlayout->setSpacing(10);

    for (int i = 0; i < widgets.size(); i++)
    {
        QString name = widgets.at(i)->name;
        TextCheckBox *widgetItem = new TextCheckBox(widgets.at(i)->showName, name);
        Setting setting = DbUtil::findSetting(name);
        if (setting.inited && setting.isShow) {
            widgetItem->setChecked(true);
        } else {
            widgetItem->setChecked(false);
        }
        widgetItems.insert(name, widgetItem);
        contentlayout->addWidget(widgetItem, i / 2, i % 2);
    }
    layout->addLayout(contentlayout);
}

void WidgetsPanel::initSignalSlots()
{
    foreach(BaseCard *widget, widgets) {
        TextCheckBox *widgetItem = this->findChild<TextCheckBox*>(widget->name);
        connect(widgetItem, SIGNAL(clicked()), this, SLOT(toggleWidgetItem()));
        connect(widget->findChild<IconButton*>("close-button"), SIGNAL(clicked()), widgetItem, SLOT(click()));
    }
}

void WidgetsPanel::initWidgets()
{
    widgets =
    {
        new SysMonitor,
        new YiYan,
        new YiWen,
        new Movie,
        new News
    };
}

void WidgetsPanel::toggleWidgetItem()
{
    foreach(BaseCard *widget, widgets) {
        if (this->findChild<TextCheckBox*>(widget->name)->isChecked()) {
            widget->show();
            DbUtil::updateSetting(widget->name, "is_show", true);
        } else {
            widget->hide();
            DbUtil::updateSetting(widget->name, "is_show", false);
        }
    }
}

WidgetsPanel::WidgetsPanel(QWidget *parent) : BasePanel (parent)
{
    widgetItems = QMap<QString, TextCheckBox*>();
    this->initWidgets();
    this->initUI();
    this->initSignalSlots();
}

QList<BaseCard*> WidgetsPanel::getWidgets()
{
    return widgets;
}

QMap<QString, TextCheckBox*> WidgetsPanel::getWidgetItems()
{
    return widgetItems;
}
