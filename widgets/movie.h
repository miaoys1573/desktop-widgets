#ifndef MOVIE_H
#define MOVIE_H

#include <base/basecard.h>

class MovieData;
class Movie : public BaseCard
{
    Q_OBJECT

private:
    QString movieTitle;
    QString movieLink;

    void initUI();
    void changeFontColor(QString color) Q_DECL_OVERRIDE;
    void updateData() Q_DECL_OVERRIDE;

    MovieData getMovieData();

public:
    Movie(QWidget *parent = 0);
};

class MovieData
{
public:
    QString title;
    QString desc;
    QString link;
    QString imgUrl;
    QString area;
    int year;
    QString director;
    QString rating;
    QStringList types;
};
#endif // MOVIE_H
