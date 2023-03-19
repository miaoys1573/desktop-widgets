#ifndef SYSMONITOR_H
#define SYSMONITOR_H

#include <base/basecard.h>

/*
 * en/eth：Ethernet
 * wl：无线网卡（WIFI）
 * ww：无线广域网卡（4G上网卡等）
 * eno：板载网卡
 * ens：热插拔网卡
 * enp：pcie网卡
 */
class SysMonitorData;
class SysMonitor : public BaseCard
{
    Q_OBJECT

private:
    void initUI();
    void changeFontColor(QString color) Q_DECL_OVERRIDE;
    void updateData() Q_DECL_OVERRIDE;

    SysMonitorData getSysMonitorData();

public:
    SysMonitor(QWidget *parent = 0);
};

class SysMonitorData
{
public:
    QString hostname;
    QString productVersion;
    QString coreNum;
    QString kernelVersion;
    QString network;
    double totalRam = 0;
    double usedRam = 0;
    double ramUsage = 0;
    double totalSwap = 0;
    double usedSwap = 0;
    double swapUsage = 0;
    double cpuUsage = 0;
};

#endif // SYSMONITOR_H
