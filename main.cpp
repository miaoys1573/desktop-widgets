#include <QApplication>
#include <qtsingleapplication.h>
#include <setting/mainsetting.h>

int main(int argc, char *argv[])
{
    QtSingleApplication app(argc, argv);
    if (app.isRunning()) {
        qApp->quit();
        return -1;
    }

    MainSetting mainSetting;
    mainSetting.showWidgets();

    return app.exec();
}
