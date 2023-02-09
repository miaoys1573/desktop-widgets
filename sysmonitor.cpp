#include "baselabel.h"
#include "baseprogress.h"
#include "sysmonitor.h"
#include <QNetworkInterface>
#include <QProcess>
#include <qhostinfo.h>

void SysMonitor::initUI()
{
    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(new BaseLabel("主机名", "hostname-label"), 0, 0, Qt::AlignLeft);
    layout1->addWidget(new BaseLabel("", "hostname"), 0, 1, Qt::AlignRight);
    layout1->addWidget(new BaseLabel("系统版本", "product-version-label"), 1, 0, Qt::AlignLeft);
    layout1->addWidget(new BaseLabel("", "product-version"), 1, 1, Qt::AlignRight);
    layout1->addWidget(new BaseLabel("内核", "kernel-version-label"), 2, 0, Qt::AlignLeft);
    layout1->addWidget(new BaseLabel("", "kernel-version"), 2, 1, Qt::AlignRight);
    layout1->addWidget(new BaseLabel("网络", "network-label"), 3, 0, Qt::AlignLeft);
    layout1->addWidget(new BaseLabel("", "network"), 3, 1, Qt::AlignRight);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addWidget(new BaseLabel("CPU使用：", "cpu-usage"), Qt::AlignLeft);
    layout2->addWidget(new BaseProgress("cpu-usage-progress"));
    layout2->addWidget(new BaseLabel("内存使用：", "ram-usage"), Qt::AlignLeft);
    layout2->addWidget(new BaseProgress("ram-usage-progress"));
    layout2->addWidget(new BaseLabel("交换空间使用：", "swap-usage"), Qt::AlignLeft);
    layout2->addWidget(new BaseProgress("swap-usage-progress"));
    layout2->setContentsMargins(0, 5, 0, 0);
    layout2->setAlignment(Qt::AlignBottom);

    layout->addLayout(layout1);
    layout->addLayout(layout2);

    this->updateData();
}

void SysMonitor::changeFontColor(QString color)
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

void SysMonitor::updateData()
{
    SysMonitorData sysMonitorData = this->getSysMonitorData();
    this->findChild<BaseLabel*>("hostname")->setText(sysMonitorData.hostname);
    this->findChild<BaseLabel*>("product-version")->setText(sysMonitorData.productVersion);
    this->findChild<BaseLabel*>("kernel-version")->setText(sysMonitorData.kernelVersion);
    this->findChild<BaseLabel*>("network")->setText(sysMonitorData.network);
    this->findChild<BaseLabel*>("cpu-usage")->setText(QString("CPU使用：%1%").arg(sysMonitorData.cpuUsage));
    this->findChild<BaseProgress*>("cpu-usage-progress")->setValue(sysMonitorData.cpuUsage);
    QString ramInfo = QString("内存使用：%1GB/%2GB - %3%")
            .arg(sysMonitorData.usedRam).arg(sysMonitorData.totalRam).arg(sysMonitorData.ramUsage);
    this->findChild<BaseLabel*>("ram-usage")->setText(ramInfo);
    this->findChild<BaseProgress*>("ram-usage-progress")->setValue(sysMonitorData.ramUsage);
    BaseLabel *swapUsageLabel = this->findChild<BaseLabel*>("swap-usage");
    BaseProgress *swapUsageProgress = this->findChild<BaseProgress*>("swap-usage-progress");
    if (sysMonitorData.totalSwap > 0) {
        QString swapInfo = QString("交换空间使用：%1GB/%2GB - %3%")
                .arg(sysMonitorData.usedSwap).arg(sysMonitorData.totalSwap).arg(sysMonitorData.swapUsage);
        swapUsageLabel->setText(swapInfo);
        swapUsageLabel->show();
        swapUsageProgress->setValue(sysMonitorData.swapUsage);
        swapUsageProgress->show();
    } else {
        swapUsageLabel->hide();
        swapUsageProgress->hide();
    }
}

QString SysMonitor::getNetwork()
{
    QString network = "无网络";
    QString networkCardPrefixes = "en,wl,ww,eno,ens,enp";
    QList<QNetworkInterface> networkInterfaces = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface networkInterface, networkInterfaces)
    {
        if (networkInterface.flags().testFlag(QNetworkInterface::IsRunning))
        {
            QString name = networkInterface.name();
            if (networkCardPrefixes.contains(name.left(2)) || networkCardPrefixes.contains(name.left(3)))
            {
                QList<QNetworkAddressEntry> networkAddressEntries = networkInterface.addressEntries();
                foreach(QNetworkAddressEntry networkAddressEntry, networkAddressEntries)
                {
                    if (QAbstractSocket::IPv4Protocol == networkAddressEntry.ip().protocol())
                    {
                        network = networkAddressEntry.ip().toString();
                    }
                }
            }
        }
    }
    return network;
}

SysMonitorData SysMonitor::getSysMonitorData()
{
    SysMonitorData sysMonitorData;
    sysMonitorData.hostname = QHostInfo::localHostName();
    sysMonitorData.productVersion = QSysInfo::productVersion();
    sysMonitorData.kernelVersion = QSysInfo::kernelVersion();
    sysMonitorData.network = this->getNetwork();

    // 数据读取
    QProcess process;
    process.start("free", QStringList("-m"));
    process.waitForFinished();
    QString line1 = process.readAllStandardOutput();
    process.start("cat", QStringList("/proc/stat"));
    process.waitForFinished();
    QString line2 = process.readLine();
    process.close();

    // 内存
    QStringList memInfo = line1.split("\n").at(1).split(" ", Qt::SkipEmptyParts);
    QStringList swapInfo = line1.split("\n").at(2).split(" ", Qt::SkipEmptyParts);
    sysMonitorData.totalRam = QString::asprintf("%.1f", memInfo.at(1).toInt() / 1024.0).toDouble();
    sysMonitorData.usedRam = QString::asprintf("%.1f", memInfo.at(2).toInt() / 1024.0).toDouble();
    sysMonitorData.ramUsage = QString::asprintf("%.1f", 100.0 * sysMonitorData.usedRam / sysMonitorData.totalRam).toDouble();
    sysMonitorData.totalSwap = QString::asprintf("%.1f", swapInfo.at(1).toInt() / 1024.0).toDouble();
    sysMonitorData.usedSwap = QString::asprintf("%.1f", swapInfo.at(2).toInt() / 1024.0).toDouble();
    sysMonitorData.swapUsage = QString::asprintf("%.1f", 100.0 * sysMonitorData.usedSwap / sysMonitorData.totalSwap).toDouble();

    // CPU
    QStringList cpuInfo = line2.split(" ", Qt::SkipEmptyParts);
    QMap<QString, int> cpuInfoMap;
    {
        cpuInfoMap["user"] = cpuInfo.at(1).toInt();
        cpuInfoMap["nice"] = cpuInfo.at(2).toInt();
        cpuInfoMap["sys"] = cpuInfo.at(3).toInt();
        cpuInfoMap["idle"] = cpuInfo.at(4).toInt();
        cpuInfoMap["iowait"] = cpuInfo.at(5).toInt();
        cpuInfoMap["hardqirq"] = cpuInfo.at(6).toInt();
        cpuInfoMap["softirq"] = cpuInfo.at(7).toInt();
        cpuInfoMap["steal"] = cpuInfo.at(8).toInt();
        cpuInfoMap["guest"] = cpuInfo.at(9).toInt();
        cpuInfoMap["guest_nice"] = cpuInfo.at(10).toInt();
    }
    int totalCpu = 0;
    for(int i = 1; i <= 10; i++) {
        totalCpu += cpuInfo.at(i).toInt();
    }
    cpuInfoMap["total"] = totalCpu;
    double cpuTotalDiff = cpuInfoMap["total"] - lastCpuInfoMap["total"];
    double cpuIdleDiff = cpuInfoMap["idle"] + cpuInfoMap["iowait"] - lastCpuInfoMap["idle"] - lastCpuInfoMap["iowait"];
    double cpuUsage = cpuTotalDiff != 0.0 ? (cpuTotalDiff - cpuIdleDiff) * 100.0 / cpuTotalDiff : 0.0;
    sysMonitorData.cpuUsage = QString::asprintf("%.1f", cpuUsage).toDouble();
    lastCpuInfoMap = cpuInfoMap;

    return sysMonitorData;
}

SysMonitor::SysMonitor(QWidget *parent) : BaseCard ("MONITOR", "系统资源", parent)
{
    lastCpuInfoMap["user"] = 0;
    lastCpuInfoMap["nice"] = 0;
    lastCpuInfoMap["sys"] = 0;
    lastCpuInfoMap["idle"] = 0;
    lastCpuInfoMap["iowait"] = 0;
    lastCpuInfoMap["hardqirq"] = 0;
    lastCpuInfoMap["softirq"] = 0;
    lastCpuInfoMap["steal"] = 0;
    lastCpuInfoMap["guest"] = 0;
    lastCpuInfoMap["guest_nice"] = 0;
    lastCpuInfoMap["total"] = 0;

    this->initUI();
    this->setTimerInterval(2000);
}
