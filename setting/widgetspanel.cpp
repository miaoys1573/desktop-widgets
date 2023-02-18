#include "widgetspanel.h"

#include <qpushbutton.h>
#include <base/dbutil.h>
#include <widgets/movie.h>
#include <widgets/news.h>
#include <widgets/sysmonitor.h>
#include <widgets/weather.h>
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
        QCheckBox *widgetItem = this->createWidgetItem(widgets.at(i)->showName, name);
        Setting setting = DbUtil::findSetting(name);
        if ((setting.inited && setting.isShow) || !setting.inited) {
            widgetItem->setChecked(true);
        } else {
            widgetItem->setChecked(false);
        }
        widgetItems.insert(name, widgetItem);
        contentlayout->addWidget(widgetItem, i / 2, i % 2);
    }
    layout->addLayout(contentlayout);
}

void WidgetsPanel::loadStyleSheet()
{

}

void WidgetsPanel::initSignalSlots()
{
    foreach(BaseCard *widget, widgets) {
        connect(this->findChild<QCheckBox*>(widget->name), SIGNAL(clicked()), this, SLOT(toggleWidgetItem()));
        connect(widget->findChild<QPushButton*>("close-button"), SIGNAL(clicked()),
                this->findChild<QCheckBox*>(widget->name), SLOT(click()));
    }
}

void WidgetsPanel::initWidgets()
{
    widgets =
    {
        new SysMonitor,
        new Weather,
        new YiYan,
        new YiWen,
        new Movie,
        new News
    };
}

QCheckBox *WidgetsPanel::createWidgetItem(QString name, QString objectName)
{
    QCheckBox *widgetItem = new QCheckBox(" " + name);
    widgetItem->setObjectName(objectName);
    widgetItem->setCursor(QCursor(Qt::PointingHandCursor));
    widgetItem->setFocusPolicy(Qt::NoFocus);

    QStringList styleSheet;
    styleSheet.append(QString("#%1{background:#FFFFFF;border-radius:5px;height:40px;}").arg(objectName));
    styleSheet.append(QString("#%1::indicator{width:40px;height:40px;background:#FFFFFF;border-radius:5px;}").arg(objectName));
    styleSheet.append(QString("#%1::indicator:checked{padding:7px;width:26px;height:26px;image:url(:/assets/icons/check.svg)}")
                      .arg(objectName));
    widgetItem->setStyleSheet(styleSheet.join(""));

    return widgetItem;
}

void WidgetsPanel::toggleWidgetItem()
{
    foreach(BaseCard *widget, widgets) {
        if (this->findChild<QCheckBox*>(widget->name)->isChecked()) {
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
    widgetItems = QMap<QString, QCheckBox*>();
    this->initWidgets();
    this->initUI();
    this->initSignalSlots();
}

QList<BaseCard *> WidgetsPanel::getWidgets()
{
    return widgets;
}

QMap<QString, QCheckBox *> WidgetsPanel::getWidgetItems()
{
    return widgetItems;
}
