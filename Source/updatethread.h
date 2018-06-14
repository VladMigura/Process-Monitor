#ifndef UPDATETHREAD_H
#define UPDATETHREAD_H

#include <QThread>
#include <windows.h>
#include <tlhelp32.h>

#include "cpuinfo.h"
#include "memoryinfo.h"

class UpdateThread : public QThread
{
    Q_OBJECT
public:
    explicit UpdateThread(QObject* parent = 0);

    void run();
    void setCounters();
    void setTable();

    bool close = false;

signals:
    void cpuCounterChanged(int);
    void memoryCounterChanged(int);
    void virtualMemoryCounterChanged(int);
    void pagingFileCounterChanged(int);

    void totalMemoryCounterChanged(int);
    void totalVirtualMemoryCounterChanged(int);
    void totalPagingFileCounterChanged(int);

    void availableMemoryCounterChanged(int);
    void availableVirtualMemoryCounterChanged(int);
    void availablePagingFileCounterChanged(int);

    void rowCountChanged(int);
    void processNameChanged(int, QString);
    void processIDChanged(int, int);
    void processPIDChanged(int, int);
    void processThreadCountChanged(int, int);
    void processPhysicalMemoryUsageChanged(int, int);
    void processPeakPhysicalMemoryUsageChanged(int, int);
    void processPagingFileUsageChanged(int, int);
    void processPeakPagingFileUsageChanged(int, int);

private:
    CPUInfo cpuInfo;
    MemoryInfo memoryInfo;
};

#endif // UPDATETHREAD_H
