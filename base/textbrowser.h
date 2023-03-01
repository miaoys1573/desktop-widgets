#ifndef TEXTBROWSER_H
#define TEXTBROWSER_H

#include <qtextbrowser.h>

class TextBrowser : public QTextBrowser
{
    Q_OBJECT

private:
    void initUI();

public:
    TextBrowser(QWidget *parent = 0);
    void setFontColor(QString color);
};

#endif // TEXTBROWSER_H
