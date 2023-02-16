#include "baseprogress.h"

void BaseProgress::loadStyleSheet()
{
    QStringList styleSheet;
    styleSheet.append(QString("#%1{background:%2;max-height:%3px;border-radius:%4px;border:0px solid %2;}")
                .arg(this->objectName()).arg("rgb(210, 210, 210)").arg(12).arg(6));
    styleSheet.append(QString("#%1:chunk{background-color:%2;border-radius:%3px;}")
                .arg(this->objectName()).arg("#FFFFFF").arg(6));
    this->setStyleSheet(styleSheet.join(""));
}

BaseProgress::BaseProgress(QString objectName, QWidget *parent) : QProgressBar(parent)
{
    this->setObjectName(objectName);
    this->setRange(0, 100);
    this->setTextVisible(false);
    this->loadStyleSheet();
}
