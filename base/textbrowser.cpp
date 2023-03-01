#include "textbrowser.h"

void TextBrowser::initUI()
{
    this->setTextInteractionFlags(Qt::NoTextInteraction);
    this->setContextMenuPolicy(Qt::NoContextMenu);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

TextBrowser::TextBrowser(QWidget *parent) : QTextBrowser (parent)
{
    this->initUI();
}

void TextBrowser::setFontColor(QString color)
{
    QString styleSheet = QString("background:rgba(0,0,0,0);color:%1").arg(color);
    this->setStyleSheet(styleSheet);
}
