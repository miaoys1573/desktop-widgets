#include "movie.h"

#include <QJsonArray>
#include <qeventloop.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>

#include <base/baselabel.h>

void Movie::initUI()
{
    MovieData movieData = this->getMovieData();

    QHBoxLayout *layout1 = new QHBoxLayout;

    BaseLabel *imageLabel = new BaseLabel("image-label");
    BaseLabel *titleLabel = new BaseLabel("", "title-label", "font-size:20px;font-weight:bold;padding:0;");
    titleLabel->setOpenExternalLinks(true);
    titleLabel->setContextMenuPolicy(Qt::NoContextMenu);
    BaseLabel *summaryLabel = new BaseLabel("summary-label");
    BaseLabel *descLabel = new BaseLabel("", "desc-label", "font-size:16px;margin-top:8px;font-style:italic");
    descLabel->setWordWrap(true);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->setContentsMargins(8, 0, 0, 0);
    layout2->addWidget(titleLabel, 0, Qt::AlignTop);
    layout2->addWidget(summaryLabel);
    layout2->addWidget(descLabel);

    layout1->addWidget(imageLabel);
    layout1->addLayout(layout2);
    layout->addLayout(layout1);

    this->updateData();
}

void Movie::changeFontColor(QString color)
{
    QPalette palette;
    palette.setColor(QPalette::WindowText, color);
    QList<BaseLabel*> labels = this->findChildren<BaseLabel*>();
    foreach (BaseLabel* label, labels)
    {
        label->setPalette(palette);
    }
    QString title = QString("<a style='color:%1;text-decoration:none' href='%2'>《%3》</a>");
    this->findChild<BaseLabel*>("title-label")->setText(title.arg(color).arg(movieLink).arg(movieTitle));
    BaseCard::changeFontColor(color);
}

void Movie::updateData()
{
    MovieData movieData = this->getMovieData();
    if (movieData.title.length() > 0) {
        // 加载网络图片
        QNetworkRequest request(movieData.imgUrl);
        QNetworkAccessManager *accessManager = new QNetworkAccessManager;
        QNetworkReply *reply = accessManager->get(request);
        QEventLoop eventLoop;
        connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
        eventLoop.exec();

        if (reply->error() == QNetworkReply::NoError) {
            QImage image = QImage::fromData(reply->readAll());
            QPixmap pixmap = QPixmap::fromImage(image).scaledToHeight(180, Qt::SmoothTransformation);
            this->findChild<BaseLabel*>("image-label")->setPixmap(pixmap);
        }

        // 右侧内容
        movieTitle = movieData.title;
        movieLink = movieData.link;
        QString summary = QString("%1  %2  %3\n%4\n%5")
                .arg(movieData.types.join("/"))
                .arg(movieData.year)
                .arg(movieData.area)
                .arg("评分：" + movieData.rating)
                .arg("导演：" + movieData.director);
        this->findChild<BaseLabel*>("summary-label")->setText(summary);
        this->findChild<BaseLabel*>("desc-label")->setText(QString("        —— %1").arg(movieData.desc));
    }
}

MovieData Movie::getMovieData()
{
    QUrl url("https://www.cikeee.com/api?app_key=pub_230a");
    QNetworkRequest request(url);
    QNetworkAccessManager *accessManager = new QNetworkAccessManager;
    QNetworkReply *reply = accessManager->get(request);
    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    MovieData movieData;
    if (reply->error() == QNetworkReply::NoError)
    {
        QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonDocument.object();
        movieData.title = jsonObject.value("mov_title").toString();
        movieData.desc = jsonObject.value("mov_text").toString();
        movieData.link = jsonObject.value("mov_link").toString();
        movieData.imgUrl = jsonObject.value("mov_pic").toString();
        movieData.area = jsonObject.value("mov_area").toString();
        movieData.year = jsonObject.value("mov_year").toInt();
        movieData.director = jsonObject.value("mov_director").toString();
        movieData.rating = jsonObject.value("mov_rating").toString();
        QJsonArray typeArray = jsonObject.value("mov_type").toArray();
        QStringList types;
        foreach (QJsonValue type, typeArray)
        {
            types.append(type.toString());
        }
        movieData.types = types;
    }
    return movieData;
}

Movie::Movie(QWidget *parent) : BaseCard ("MOVIE", "每日电影", parent)
{
    this->initUI();
    this->setTimerInterval(1000 * 60 * 60 * 12);
}
