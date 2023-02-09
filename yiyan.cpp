#include "baselabel.h"
#include "yiyan.h"

#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

void YiYan::initUI()
{
    YiYanData yiYanData = this->getYiYanData();

    this->setFixedHeight(240);
    layout->addWidget(new BaseLabel("『", "content-prefix-label", "margin-top:15px;"), 0, Qt::AlignLeft);
    BaseLabel *contentLabel = new BaseLabel("美人自刎乌江岸，战火曾烧赤壁山，山山山美人自刎乌江岸，战火曾烧赤壁山，山山山", "content-label");
    contentLabel->setWordWrap(true);
    contentLabel->setStyleSheet("font-size:24px;font-weight:bold;padding:10px;border:1px solid red;");
    layout->addWidget(contentLabel);
    layout->addWidget(new BaseLabel("』", "content-suffix-label"), 0, Qt::AlignRight);
    BaseLabel *fromLabel = new BaseLabel(yiYanData.from, "from-label", "margin-top:15px;");
    fromLabel->setWordWrap(true);
    layout->addWidget(fromLabel, 0, Qt::AlignRight);
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
    this->findChild<BaseLabel*>("content-label")->setText(yiYanData.content);
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
