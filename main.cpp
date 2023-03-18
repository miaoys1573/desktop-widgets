#include <QApplication>
#include <qfontdatabase.h>
#include <qtsingleapplication.h>
#include <setting/mainsetting.h>

int main(int argc, char *argv[])
{
    QtSingleApplication app(argc, argv);
    if (app.isRunning()) {
        qApp->quit();
        return -1;
    }

    // 独立于不同系统风格，视觉统一化处理
    int fontId = QFontDatabase::addApplicationFont(":/assets/fonts/HarmonyOS_Sans_SC_Regular.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    app.setFont(QFont(fontFamily));
    QPalette palette = app.palette();
    palette.setColor(QPalette::WindowText, QColor(100, 100, 100));
    app.setPalette(palette);

    MainSetting mainSetting;
    mainSetting.showWidgets();

    return app.exec();
}
