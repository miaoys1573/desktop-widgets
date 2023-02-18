#ifndef NEWS_H
#define NEWS_H

#include <base/basecard.h>

class NewsData;
class News : public BaseCard
{
    Q_OBJECT

private:
    void initUI();
    void changeFontColor(QString color) Q_DECL_OVERRIDE;
    void updateData() Q_DECL_OVERRIDE;

    QList<NewsData> getNewsData();

public:
    News(QWidget *parent = 0);
};

class NewsData
{
public:
    QString title;
    QString digest;
    QString showTime;
};

#endif // NEWS_H
