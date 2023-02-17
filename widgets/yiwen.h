#ifndef YIWEN_H
#define YIWEN_H

#include <base/basecard.h>

class YiWen : public BaseCard
{
    Q_OBJECT

private:
    void initUI();
    void changeFontColor(QString color) Q_DECL_OVERRIDE;
    void updateData() Q_DECL_OVERRIDE;

    QString getYiWenData();

public:
    YiWen(QWidget *parent = 0);
};

#endif // YIWEN_H
