#ifndef BASEPROGRESS_H
#define BASEPROGRESS_H

#include <qprogressbar.h>

class BaseProgress : public QProgressBar
{

private:
    void loadStyleSheet();

public:
    BaseProgress(QString objectName, QWidget *parent = 0);
};

#endif // BASEPROGRESS_H
