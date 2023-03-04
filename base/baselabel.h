#ifndef BASELABEL_H
#define BASELABEL_H

#include <qlabel.h>

class BaseLabel : public QLabel
{
    Q_OBJECT

private:
    void setFontSize(int size);

public:
    enum type {
        TitleLabel = 1,
        ContentLabel = 2
    };
    Q_ENUM(type);

    BaseLabel(QWidget *parent = 0);
    BaseLabel(QString objectName, QWidget *parent = 0);
    BaseLabel(QString text, QString objectName, QWidget *parent = 0);
    BaseLabel(QString text, QString objectName, QString styleSheet, QWidget *parent = 0);
    BaseLabel(QString text, QString objectName, BaseLabel::type labeltype, QWidget *parent = 0);
    BaseLabel(QString text, QString objectName, BaseLabel::type labeltype, QString styleSheet, QWidget *parent = 0);
};

#endif // BASELABEL_H
