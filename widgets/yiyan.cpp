#include "yiyan.h"

#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <qtextbrowser.h>

#include <base/baselabel.h>

void YiYan::initUI()
{
    BaseLabel *contentLabel = new BaseLabel("", "content-label");
    contentLabel->setFixedHeight(160);
    contentLabel->setWordWrap(true);
    contentLabel->setTextInteractionFlags(Qt::NoTextInteraction);
    contentLabel->setTextFormat(Qt::RichText);
    BaseLabel *fromLabel = new BaseLabel("", "from-label");

    layout->addWidget(contentLabel, 0, Qt::AlignVCenter);
    layout->addWidget(fromLabel, 0, Qt::AlignRight);

    this->updateData();
}

void YiYan::changeFontColor(QString color)
{
    QPalette palette;
    palette.setColor(QPalette::WindowText, color);
    QList<BaseLabel*> labels = this->findChildren<BaseLabel*>();
    foreach (BaseLabel* label, labels)
    {
        label->setPalette(palette);
    }
    BaseCard::changeFontColor(color);
}

void YiYan::updateData()
{
    YiYanData yiYanData = this->getYiYanData();
    QString htmlContent = QString("<div style=\"font-size:24px;font-weight:bold;text-indent:48px;\">%1</div>");
    this->findChild<BaseLabel*>("content-label")->setText(htmlContent.arg(yiYanData.content));
    this->findChild<BaseLabel*>("from-label")->setText(yiYanData.from);
}

YiYanData YiYan::getYiYanData()
{
    QUrl url("https://v1.hitokoto.cn/?c=d&c=i&c=k&lang=cn");
    QNetworkRequest request(url);
    QNetworkAccessManager *accessManager = new QNetworkAccessManager;
    QNetworkReply *reply = accessManager->get(request);
    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    YiYanData yiYanData;
    if (reply->error() == QNetworkReply::NoError)
    {
        QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonDocument.object();
        yiYanData.content = jsonObject.value("hitokoto").toString();
        QString from = "—— " + (jsonObject.value("from_who").isNull() ? "" : jsonObject.value("from_who").toString())
                + "「" + jsonObject.value("from").toString() + "」";
        yiYanData.from = from;
    }
    return yiYanData;
}

YiYan::YiYan(QWidget *parent) : BaseCard("YIYAN", "一言", parent)
{
    this->initUI();
    this->setTimerInterval(1000 * 60 * 30);
    this->setShowRefreshButton(true);
}
