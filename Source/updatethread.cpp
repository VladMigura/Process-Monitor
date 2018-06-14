#include "updatethread.h"

UpdateThread::UpdateThread(QObject* parent) : QThread(parent)
{
}

void UpdateThread::run()
{
    while(true)
    {
        if(close) break;
        setCounters();
        setTable();
        Sleep(1000);
    }
}

void UpdateThread::setCounters()
{
    memoryInfo.updateInfo();

    emit cpuCounterChanged(cpuInfo.getTotalUsage());
    emit memoryCounterChanged(memoryInfo.getPhysicalMemoryUsage());
    emit virtualMemoryCounterChanged(memoryInfo.getVirtualMemoryUsage());
    emit pagingFileCounterChanged(memoryInfo.getPagingFileUsage());

    emit totalMemoryCounterChanged(memoryInfo.getTotalPhysicalMemory());
    emit totalVirtualMemoryCounterChanged(memoryInfo.getTotalVirtualMemory());
    emit totalPagingFileCounterChanged(memoryInfo.getTotalPagingFile());

    emit availableMemoryCounterChanged(memoryInfo.getAvailablePhysicalMemory());
    emit availableVirtualMemoryCounterChanged(memoryInfo.getAvailableVirtualMemory());
    emit availablePagingFileCounterChanged(memoryInfo.getAvailablePagingFile());
}

void UpdateThread::setTable()
{
    char processName[260];
    int pid;
    int count=0;

    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    bool bIsok = false;
    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(processEntry);

    bIsok = Process32First(hProcessSnap, &processEntry);
    while(bIsok)
    {
        count++;
        bIsok = Process32Next(hProcessSnap, &processEntry);
    }

    emit rowCountChanged(count-1);

    bIsok = Process32First(hProcessSnap, &processEntry);

    for(int i=0; bIsok; i++)
    {
        if(i == 0) bIsok = Process32Next(hProcessSnap, &processEntry);
        WideCharToMultiByte(CP_ACP, 0, processEntry.szExeFile, -1, processName, 260, NULL, NULL);

        pid = processEntry.th32ProcessID;

        emit processNameChanged(i, processName);
        emit processIDChanged(i, processEntry.th32ProcessID);
        emit processPIDChanged(i, processEntry.th32ParentProcessID);
        emit processThreadCountChanged(i, processEntry.cntThreads);
        emit processPhysicalMemoryUsageChanged(i,
        memoryInfo.getProcessPhysicalMemoryUsage(memoryInfo.getProcessHandle(pid)));

        emit processPeakPhysicalMemoryUsageChanged(i,
        memoryInfo.getProcessPeakPhysicalMemoryUsage(memoryInfo.getProcessHandle(pid)));

        emit processPagingFileUsageChanged(i,
        memoryInfo.getProcessPagingFileUsage(memoryInfo.getProcessHandle(pid)));

        emit processPeakPagingFileUsageChanged(i,
        memoryInfo.getProcessPeakPagingFileUsage(memoryInfo.getProcessHandle(pid)));

        bIsok = Process32Next(hProcessSnap, &processEntry);
    }
    CloseHandle(hProcessSnap);
}
