#ifndef YIYAN_H
#define YIYAN_H

#include <base/basecard.h>

class YiYanData;
class YiYan : public BaseCard
{
    Q_OBJECT

private:
    void initUI();
    void changeFontColor(QString color) Q_DECL_OVERRIDE;
    void updateData() Q_DECL_OVERRIDE;

    YiYanData getYiYanData();

public:
    YiYan(QWidget *parent = 0);
};

class YiYanData
{
public:
    QString content;
    QString from;
};

#endif // YIYAN_H
