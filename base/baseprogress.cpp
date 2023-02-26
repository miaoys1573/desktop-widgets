#include "baseprogress.h"
#include "constants.h"

void BaseProgress::initUI()
{
    this->setRange(0, 100);
    this->setTextVisible(false);
}

void BaseProgress::loadStyleSheet()
{
    QStringList styleSheet;
    styleSheet.append(QString("QProgressBar{background:%1;max-height:%2px;border-radius:%3px;border:0px solid %1;}")
                .arg(Constants::MAIN_BACKGROUND_4).arg(Constants::PROGRESS_HEIGHT).arg(Constants::PROGRESS_HEIGHT / 2));
    styleSheet.append(QString("QProgressBar:chunk{background-color:%1;border-radius:%2px;}")
                .arg(Constants::MAIN_BACKGROUND_1).arg(Constants::PROGRESS_HEIGHT / 2));
    this->setStyleSheet(styleSheet.join(""));
}

BaseProgress::BaseProgress(QString objectName, QWidget *parent) : QProgressBar(parent)
{
    this->setObjectName(objectName);
    this->initUI();
    this->loadStyleSheet();
}
