#ifndef DBUTIL_H
#define DBUTIL_H

#include "basesetting.h"
#include <qsqldatabase.h>

class Setting;
class DbUtil : QObject
{
private:
    static QSqlDatabase initDb();

public:
    DbUtil();
    static int countSetting();
    static Setting findSetting(QString name);
    static void updateSetting(QString name, QString key, int value);
    static void updateSetting(QString name, QString key, QString value);
    static void updateSetting(QString name, QStringList keys, QStringList values);
};

class Setting
{
public:
    bool inited = false;
    QString name;
    int positionX;
    int positionY;
    BaseSetting::BackgroundType backgroundType;
    int backgroundPureColorIndex;
    int backgroundPureColorTransparence;
    int backgroundGradientColorIndex;
    int fontColorIndex;
    bool isShow;
};

#endif // DBUTIL_H

