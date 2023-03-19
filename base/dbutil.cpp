#include "dbutil.h"
#include <QDir>
#include <QMetaEnum>
#include <QSqlQuery>
#include <QVariant>

QSqlDatabase DbUtil::initDb()
{
    QString dirName;
#ifdef Q_OS_WIN
    dirName = QString("%1/AppData/Local/desktop-widgets").arg(QDir::homePath());
#else
    dirName = QString("%1/.config/desktop-widgets").arg(QDir::homePath());
#endif

    QSqlDatabase database;
    QString connectionName = "SQLLITE_CONNECTION";
    if (QSqlDatabase::contains(connectionName)) {
        database = QSqlDatabase::database("SQLLITE_CONNECTION");
    } else {
        database = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        QDir dir(dirName);
        if (!dir.exists()) {
            dir.mkdir(dirName);
        }
        database.setDatabaseName(dirName + "/data.db");
        database.open();
        QSqlQuery query(database);
        query.exec(QString(R"(CREATE TABLE 'setting' (
                   'name' text(100) NOT NULL,
                   'position_x' integer(10),
                   'position_y' integer(10),
                   'background_type' integer(2),
                   'background_pure_color_index' integer(4),
                   'background_pure_color_transparence' integer(4),
                   'background_gradient_color_index' integer(4),
                   'font_color_index' integer(4),
                   'is_show' integer(1),
                   PRIMARY KEY ('name')
                 );)"));
    }
    if (!database.isOpen()) {
        database.open();
    }
    return database;
}

DbUtil::DbUtil()
{

}

int DbUtil::countSetting()
{
    QSqlDatabase database = initDb();
    QSqlQuery query(database);
    query.exec(QString("SELECT count(0) FROM setting"));
    int count = 0;
    if (query.next()){
        count = query.value(0).toInt();
    }
    return count;
}

Setting DbUtil::findSetting(QString name)
{
    QSqlDatabase database = initDb();
    QStringList list = database.tables();
    QSqlQuery query(database);
    query.exec(QString("SELECT * FROM setting WHERE name = '%1';").arg(name));
    Setting setting;
    if (query.next()){
        setting.inited = true;
        setting.name = name;
        setting.positionX = query.value(1).toInt();
        setting.positionY = query.value(2).toInt();
        setting.backgroundType = query.value(3).isNull() ? BaseSetting::PureColor : (BaseSetting::BackgroundType)query.value(3).toInt();
        setting.backgroundPureColorIndex = query.value(4).isNull() ? 0 : query.value(4).toInt();
        setting.backgroundPureColorTransparence = query.value(5).isNull() ? 80 : query.value(5).toInt();
        setting.backgroundGradientColorIndex = query.value(6).isNull() ? 0 : query.value(6).toInt();
        setting.fontColorIndex = query.value(7).isNull() ? 0 : query.value(7).toInt();
        setting.isShow = query.value(7).isNull() ? true : query.value(8).toBool();
    }
    return setting;
}

void DbUtil::updateSetting(QString name, QString key, int value)
{
    updateSetting(name, key, QString::number(value));
}

void DbUtil::updateSetting(QString name, QString key, QString value)
{
    if (!name.isEmpty())
    {
        QSqlDatabase database = initDb();
        QStringList list = database.tables();
        QSqlQuery query(database);
        query.exec(QString("SELECT * FROM setting WHERE name = '%1';").arg(name));
        if (!query.next()) {
            query.exec(QString("INSERT INTO setting (name, %2) VALUES('%1', '%3');").arg(name).arg(key).arg(value));
        } else {
            query.exec(QString("UPDATE setting SET %2 = '%3' WHERE name='%1';").arg(name).arg(key).arg(value));
        }
    }
}

void DbUtil::updateSetting(QString name, QStringList keys, QStringList values)
{
    if (!name.isEmpty())
    {
        QSqlDatabase database = initDb();
        QSqlQuery query(database);
        query.exec(QString("SELECT * FROM setting WHERE name = '%1';").arg(name));
        if (!query.next()) {
            QStringList sqlPart;
            foreach (QString key, keys) {
                sqlPart.append("?");
            }
            query.prepare(QString("INSERT INTO setting (name, %1) VALUES(?, %2);").arg(keys.join(",")).arg(sqlPart.join(",")));
            query.bindValue(0, name);
            for (int i = 1; i <= values.size(); i++) {
                query.bindValue(1, values.at(i - 1));
            }
            query.exec();
        } else {
            QStringList sqlPart;
            for (int i = 0; i < values.size(); i++) {
                sqlPart.append(keys.at(i) + "='" + values.at(i) + "'");
            }
            query.exec(QString("UPDATE setting SET %2 WHERE name='%1';").arg(name).arg(sqlPart.join(",")));
        }
    }
}
