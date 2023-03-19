#include "sysmonitor.h"
#include <QNetworkInterface>
#include <QProcess>
#include <qhostinfo.h>
#include <base/baselabel.h>
#include <base/baseprogress.h>
#include <base/constants.h>

QVector<double> _getLinuxMemUsage()
{
    // 数据读取
    QProcess process;
    process.start("free", QStringList("-m"));
    process.waitForFinished();
    QString line = process.readAllStandardOutput();
    process.close();

    // 内存
    QStringList memInfo = line.split("\n").at(1).split(" ", QString::SkipEmptyParts);
    QStringList swapInfo = line.split("\n").at(2).split(" ", QString::SkipEmptyParts);
    double totalRam = QString::asprintf("%.1f", memInfo.at(1).toInt() / 1024.0).toDouble();
    double usedRam = QString::asprintf("%.1f", memInfo.at(2).toInt() / 1024.0).toDouble();
    double ramUsage = QString::asprintf("%.1f", 100.0 * usedRam / totalRam).toDouble();
    double totalSwap = QString::asprintf("%.1f", swapInfo.at(1).toInt() / 1024.0).toDouble();
    double usedSwap = QString::asprintf("%.1f", swapInfo.at(2).toInt() / 1024.0).toDouble();
    double swapUsage = QString::asprintf("%.1f", 100.0 * usedSwap / totalSwap).toDouble();

    QVector<double> result = {totalRam, usedRam, ramUsage, totalSwap, usedSwap, swapUsage};
    return result;
}

QMap<QString, int> lastCpuInfoMap = {
    std::map<QString,int>::value_type("user", 0),
    std::map<QString,int>::value_type("nice", 0),
    std::map<QString,int>::value_type("sys", 0),
    std::map<QString,int>::value_type("idle", 0),
    std::map<QString,int>::value_type("iowait", 0),
    std::map<QString,int>::value_type("hardqirq", 0),
    std::map<QString,int>::value_type("softirq", 0),
    std::map<QString,int>::value_type("steal", 0),
    std::map<QString,int>::value_type("guest", 0),
    std::map<QString,int>::value_type("guest_nice", 0),
    std::map<QString,int>::value_type("total", 0)
};
double _getLinuxCpuUsage()
{
    // 数据读取
    QProcess process;
    process.start("cat", QStringList("/proc/stat"));
    process.waitForFinished();
    QString line = process.readLine();
    process.close();

    // CPU
    QStringList cpuInfo = line.split(" ", QString::SkipEmptyParts);
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
    lastCpuInfoMap = cpuInfoMap;
    return QString::asprintf("%.1f", cpuUsage).toDouble();
}

#ifdef Q_OS_WIN
#include <Pdh.h>
#include <PdhMsg.h>
QVector<double> _getWinMemUsage()
{
    QVector<double> result = {0, 0, 0};
    MEMORYSTATUSEX memsStat;
    memsStat.dwLength = sizeof(memsStat);
    if (GlobalMemoryStatusEx(&memsStat))
    {
        double freeRam = memsStat.ullAvailPhys / (1024.0 * 1024.0 * 1024.0);
        double totalRam = memsStat.ullTotalPhys / (1024.0 * 1024.0 * 1024.0);
        double usedRam = QString::asprintf("%.1f", totalRam - freeRam).toDouble();
        totalRam = QString::asprintf("%.1f", totalRam).toDouble();
        double ramUsage = QString::asprintf("%.1f", 100.0 * usedRam / totalRam).toDouble();
        result = {totalRam, usedRam, ramUsage};
    }
    return result;
}

PDH_RAW_COUNTER lastRawCounter;
double _getWinCpuUsage()
{
    HQUERY query;
    HCOUNTER counter;
    DWORD counterType;
    PDH_RAW_COUNTER rawCounter;

    // 开始查询
    PdhOpenQuery(NULL, 0, &query);
    const wchar_t* queryStr = L"\\Processor Information(_Total)\\% Processor Utility";
    PdhAddCounter(query, queryStr, NULL, &counter);
    PdhCollectQueryData(query);
    PdhGetRawCounterValue(counter, &counterType, &rawCounter);

    PDH_FMT_COUNTERVALUE fmtValue;
    // 计算使用率
    PdhCalculateCounterFromRawValue(counter, PDH_FMT_DOUBLE, &rawCounter, &lastRawCounter, &fmtValue);
    // 关闭查询
    PdhCloseQuery(query);

    lastRawCounter = rawCounter;
    double cpuUsage = fmtValue.doubleValue;
    return QString::asprintf("%.1f", cpuUsage).toDouble();
}
#endif

QString _getNetwork()
{
    QString network = "无网络";
    QString networkCardPrefixes = "en,eth,wl,ww,eno,ens,enp";
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

void SysMonitor::initUI()
{
    SysMonitorData sysMonitorData = this->getSysMonitorData();

    if (sysMonitorData.totalSwap == 0) {
        QGridLayout *layout1 = new QGridLayout;
        layout1->addWidget(new BaseLabel("主机名", "hostname-label"), 0, 0, Qt::AlignLeft);
        layout1->addWidget(new BaseLabel("hostname"), 0, 1, Qt::AlignRight);
        layout1->addWidget(new BaseLabel("系统版本", "product-version-label"), 1, 0, Qt::AlignLeft);
        layout1->addWidget(new BaseLabel("product-version"), 1, 1, Qt::AlignRight);
        layout1->addWidget(new BaseLabel("内核", "kernel-version-label"), 2, 0, Qt::AlignLeft);
        layout1->addWidget(new BaseLabel("kernel-version"), 2, 1, Qt::AlignRight);
        layout1->addWidget(new BaseLabel("网络", "network-label"), 3, 0, Qt::AlignLeft);
        layout1->addWidget(new BaseLabel("network"), 3, 1, Qt::AlignRight);

        layout->addLayout(layout1);
    } else {
        QHBoxLayout *layout1 = new QHBoxLayout;
        layout1->addWidget(new BaseLabel("主机名\n系统版本\n内核\n网络", "title-label"), Qt::AlignLeft);
        BaseLabel *valueLabel = new BaseLabel("value-label");
        valueLabel->setTextFormat(Qt::RichText);
        valueLabel->setTextInteractionFlags(Qt::NoTextInteraction);
        layout1->addWidget(valueLabel, Qt::AlignRight);

        layout->addLayout(layout1);
    }

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addWidget(new BaseLabel("CPU使用：", "cpu-usage"), Qt::AlignLeft);
    layout2->addWidget(new BaseProgress("cpu-usage-progress"));
    layout2->addWidget(new BaseLabel("内存使用：", "ram-usage"), Qt::AlignLeft);
    layout2->addWidget(new BaseProgress("ram-usage-progress"));
    layout2->addWidget(new BaseLabel("交换空间使用：", "swap-usage"), Qt::AlignLeft);
    layout2->addWidget(new BaseProgress("swap-usage-progress"));
    layout2->setAlignment(Qt::AlignBottom);
    layout2->setContentsMargins(0, 0, 0, 2);

    layout->addLayout(layout2);
    layout->setContentsMargins(Constants::WIDGET_RADIUS, 0, Constants::WIDGET_RADIUS, 10);

    if (sysMonitorData.totalSwap == 0) {
        layout2->setContentsMargins(0, 12, 0, 5);
        layout->setContentsMargins(Constants::WIDGET_RADIUS, 5, Constants::WIDGET_RADIUS, Constants::WIDGET_RADIUS);
    }
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

    if (sysMonitorData.totalSwap == 0) {
        this->findChild<BaseLabel*>("hostname")->setText(sysMonitorData.hostname);
        this->findChild<BaseLabel*>("product-version")->setText(sysMonitorData.productVersion);
        this->findChild<BaseLabel*>("kernel-version")->setText(sysMonitorData.kernelVersion);
        this->findChild<BaseLabel*>("network")->setText(sysMonitorData.network);
    } else {
        QString valueTemplate = "<div style='text-align:right;'>%1</div>";
        QStringList value;
        value.append(valueTemplate.arg(sysMonitorData.hostname));
        value.append(valueTemplate.arg(sysMonitorData.productVersion));
        value.append(valueTemplate.arg(sysMonitorData.kernelVersion));
        value.append(valueTemplate.arg(sysMonitorData.network));
        this->findChild<BaseLabel*>("value-label")->setText(value.join(""));
    }

    this->findChild<BaseLabel*>("cpu-usage")->setText(QString("CPU使用：%1%").arg(sysMonitorData.cpuUsage));
    this->findChild<BaseProgress*>("cpu-usage-progress")->setValue(sysMonitorData.cpuUsage);

    // 内存
    QString ramInfo = QString("内存使用：%1GB/%2GB - %3%")
            .arg(sysMonitorData.usedRam).arg(sysMonitorData.totalRam).arg(sysMonitorData.ramUsage);
    this->findChild<BaseLabel*>("ram-usage")->setText(ramInfo);
    this->findChild<BaseProgress*>("ram-usage-progress")->setValue(sysMonitorData.ramUsage);

    // 交换空间
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

SysMonitorData SysMonitor::getSysMonitorData()
{
    SysMonitorData sysMonitorData;
    sysMonitorData.hostname = QHostInfo::localHostName();
    sysMonitorData.productVersion = QSysInfo::productVersion();
    sysMonitorData.kernelVersion = QSysInfo::kernelVersion();
    sysMonitorData.network = _getNetwork();

#ifdef Q_OS_LINUX
    QVector<double> memUsage = _getLinuxMemUsage();
    double cpuUsage = _getLinuxCpuUsage();
    sysMonitorData.totalRam = memUsage.at(0);
    sysMonitorData.usedRam = memUsage.at(1);
    sysMonitorData.ramUsage = memUsage.at(2);
    sysMonitorData.totalSwap = memUsage.at(3);
    sysMonitorData.usedSwap = memUsage.at(4);
    sysMonitorData.swapUsage = memUsage.at(5);
    sysMonitorData.cpuUsage = cpuUsage;
#elif defined(Q_OS_WIN)
    QVector<double> memUsage = _getWinMemUsage();
    double cpuUsage = _getWinCpuUsage();
    sysMonitorData.totalRam = memUsage.at(0);
    sysMonitorData.usedRam = memUsage.at(1);
    sysMonitorData.ramUsage = memUsage.at(2);
    sysMonitorData.cpuUsage = cpuUsage;
#endif

    return sysMonitorData;
}

SysMonitor::SysMonitor(QWidget *parent) : BaseCard ("MONITOR", "系统资源", parent)
{
    this->initUI();
    this->setTimerInterval(2000);
}
