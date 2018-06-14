#ifndef MEMORYINFO_H
#define MEMORYINFO_H

#include <windows.h>
#include <psapi.h>

class MemoryInfo
{
    long COEFFICIENT_OF_DIVISION_MB = 1048576;

    MEMORYSTATUSEX memoryStatus;
    PROCESS_MEMORY_COUNTERS processMemory;

public:
    MemoryInfo();

    void updateInfo();

    int getTotalPhysicalMemory();
    int getPhysicalMemoryUsage();
    int getAvailablePhysicalMemory();
    int getTotalVirtualMemory();
    int getVirtualMemoryUsage();
    int getAvailableVirtualMemory();
    int getBusyVirtualMemory();
    int getTotalPagingFile();
    int getPagingFileUsage();
    int getAvailablePagingFile();
    int getBusyPagingFile();

    HANDLE getProcessHandle(int pid);
    int getProcessPhysicalMemoryUsage(HANDLE hProcess);
    int getProcessPeakPhysicalMemoryUsage(HANDLE hProcess);
    int getProcessPagingFileUsage(HANDLE hProcess);
    int getProcessPeakPagingFileUsage(HANDLE hProcess);
};

#endif // MEMORYINFO_H
