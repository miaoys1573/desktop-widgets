#ifndef SYSMONITOR_H
#define SYSMONITOR_H

#include <base/basecard.h>

/*
 * en：Ethernet
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
    QMap<QString, int> lastCpuInfoMap;

    void initUI();
    void changeFontColor(QString color) Q_DECL_OVERRIDE;
    void updateData() Q_DECL_OVERRIDE;

    QString getNetwork();
    SysMonitorData getSysMonitorData();

public:
    SysMonitor(QWidget *parent = 0);
};

class SysMonitorData
{
public:
    QString hostname;
    QString productVersion;
    QString kernelVersion;
    QString network;
    double totalRam;
    double usedRam;
    double ramUsage;
    double totalSwap = 0;
    double usedSwap;
    double swapUsage;
    double cpuUsage;
};

#endif // SYSMONITOR_H
