#include "news.h"

#include <base/baselabel.h>
#include <base/constants.h>

#include <qeventloop.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>
#include <qtextbrowser.h>

void News::initUI()
{
    this->setFixedHeight(Constants::WIDGET_HEIGHT *2 + 10);

    QTextBrowser *contentTextBrowser = new QTextBrowser();
    contentTextBrowser->setObjectName("content-text-browser");
    contentTextBrowser->setTextInteractionFlags(Qt::NoTextInteraction);
    contentTextBrowser->setContextMenuPolicy(Qt::NoContextMenu);
    contentTextBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    layout->addWidget(contentTextBrowser);

    this->updateData();
}

void News::changeFontColor(QString color)
{
    QString styleSheet = QString("background:rgba(0,0,0,0);color:%1").arg(color);
    this->findChild<QTextBrowser*>("content-text-browser")->setStyleSheet(styleSheet);
    BaseCard::changeFontColor(color);
}

void News::updateData()
{
    QList<NewsData> newsDataList = this->getNewsData();
    QStringList content;
    if (newsDataList.size() > 0) {
        for (int i = 0; i < newsDataList.size(); i++) {
            NewsData newsData = newsDataList.at(i);
            content.append(QString("<div style='text-align:center;margin-top:%2px'>%1</div>")
                           .arg(newsData.showTime).arg(i == 0 ? 0 : 10));
            content.append(QString("<div style='font-size:%1px;text-align:center;font-weight:bold;margin-top:8px'>%2</div>")
                           .arg(Constants::TITLE_FONT_SIZE)
                           .arg(newsData.title));
            content.append(QString("<div style='font-size:%1px;text-indent:30px;margin-top:8px'>%2</div>")
                           .arg(Constants::CONTENT_FONT_SIZE)
                           .arg(newsData.digest));
            content.append("<br>");
        }
        this->findChild<QTextBrowser*>("content-text-browser")->setText(content.join(""));
    }
}

QList<NewsData> News::getNewsData()
{
    QUrl url("https://newsinfo.eastmoney.com/kuaixun/v2/api/list?column=102&limit=20&p=1");
    QNetworkRequest request(url);
    QNetworkAccessManager *accessManager = new QNetworkAccessManager;
    QNetworkReply *reply = accessManager->get(request);
    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    QList<NewsData> newsDataList;
    if (reply->error() == QNetworkReply::NoError)
    {
        QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonDocument.object();
        QJsonArray newsArray = jsonObject.value("news").toArray();
        foreach (QJsonValue news, newsArray)
        {
            QJsonObject newsObject = news.toObject();
            NewsData newsData;
            newsData.title = newsObject.value("title").toString();
            newsData.digest = newsObject.value("digest").toString();
            newsData.showTime = newsObject.value("showtime").toString();
            newsDataList.append(newsData);
        }
    }
    return newsDataList;
}

News::News(QWidget *parent) : BaseCard ("NEWS", "财经资讯", parent)
{
    this->initUI();
    this->setTimerInterval(1000 * 60 * 2);
    this->setShowRefreshButton(true);
}
