#include "basepanel.h"
#include "basewidget.h"
#include "sysmonitor.h"
#include "mainsetting.h"
#include "yiyan.h"
#include <qsharedmemory.h>
#include <QApplication>
#include <qtsingleapplication.h>

int main(int argc, char *argv[])
{
    QtSingleApplication app(argc, argv);
    if (app.isRunning()) {
        qApp->quit();
        return -1;
    }

    MainSetting mainSetting;
    mainSetting.showWidgets();
//    YiYan yiyan;
//    yiyan.show();

    return app.exec();
}
