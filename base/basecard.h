#ifndef BASECARD_H
#define BASECARD_H

#include "basebutton.h"
#include "basesetting.h"
#include <qmap.h>

class BaseCard : public BaseWidget
{
    Q_OBJECT

private:
    bool showRefreshButton;
    int backgroundPureColorIndex;
    int backgroundPureColorAlpha;
    int timerId = -1;

    void initUI();
    void initSignalSlots();

    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private slots:
    void refresh();
    void showSettingPanel();
    void changeBackground();
    void changeBackgroundAlpha(int transparence);
    void changeFontColor();

protected:
    BaseSetting *_setting;

    // 设置定时频率后，会同时启动定时器
    void setTimerInterval(int interval);

    // 如需支持字体颜色改变，需重写该函数
    virtual void changeFontColor(QString color);
    // 该函数需要子类重写，启动定时器后，会多次调用该函数
    virtual void updateData();

    void setShowRefreshButton(bool flag);

public:
    QString showName;
    QVBoxLayout *layout;

    BaseCard(QString name, QString showName, QWidget *parent = 0);
    BaseCard(QString name, QString showName, BaseSetting *setting, QWidget *parent = 0);

    BaseSetting* getSetting();
};

#endif // BASECARD_H
