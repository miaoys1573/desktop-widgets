#include "yiwen.h"

#include <QDesktopServices>
#include <qeventloop.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>

#include <base/baselabel.h>
#include <base/constants.h>
#include <base/textbrowser.h>

void YiWen::initUI()
{
    this->setShowRefreshButton(true);
    this->setFixedHeight(Constants::WIDGET_HEIGHT *2 + 10);

    layout->addWidget(new TextBrowser());
}

void YiWen::changeFontColor(QString color)
{
    this->findChild<TextBrowser*>()->setFontColor(color);
    BaseCard::changeFontColor(color);
}

void YiWen::updateData()
{
    QString content = this->getYiWenData();
    if (content.length() > 0)
    {
        this->findChild<TextBrowser*>()->setHtml(content);
    }
}

QString YiWen::getYiWenData()
{
    QUrl url("https://meiriyiwen.com/random");
    QNetworkRequest request(url);
    QNetworkAccessManager *accessManager = new QNetworkAccessManager;
    QNetworkReply *reply = accessManager->get(request);
    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        QStringList data = QString(reply->readAll()).split("\n");
        bool start = false;
        QStringList result;
        foreach (QString line, data)
        {
            if (line.contains("<div id=\"article_show\">"))
            {
                start = true;
            }
            if (line.contains("<div id=\"bdshare"))
            {
                start = false;
            }
            if (start)
            {
                line = line.replace("class=\"article_text\"", QString("style='font-size:%1px;'").arg(Constants::CONTENT_FONT_SIZE));
                result.append(line);
            }
        }
        result.append("<p style='text-align:center;'>— End —</p>");
        return result.join("");
    }
    return "";
}

YiWen::YiWen(QWidget *parent) : BaseCard("YIWEN", "每日一文", parent)
{
    this->initUI();
    this->setTimerInterval(1000 * 60 * 60 * 24);
}
