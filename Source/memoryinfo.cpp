#include "memoryinfo.h"

MemoryInfo::MemoryInfo()
{
    memoryStatus.dwLength = sizeof(memoryStatus);
}

void MemoryInfo::updateInfo()
{
    GlobalMemoryStatusEx(&memoryStatus);
}

int MemoryInfo::getTotalPhysicalMemory()
{
    return (int)(memoryStatus.ullTotalPhys / COEFFICIENT_OF_DIVISION_MB);
}

int MemoryInfo::getPhysicalMemoryUsage()
{
    return (int)memoryStatus.dwMemoryLoad;
}

int MemoryInfo::getAvailablePhysicalMemory()
{
    return (int)(memoryStatus.ullAvailPhys / COEFFICIENT_OF_DIVISION_MB);
}

int MemoryInfo::getTotalVirtualMemory()
{
    return (int)(memoryStatus.ullTotalVirtual / COEFFICIENT_OF_DIVISION_MB);
}

int MemoryInfo::getVirtualMemoryUsage()
{
    double total = getTotalVirtualMemory();
    double available = getAvailableVirtualMemory();
    double usage = (total - available) / total * 100;
    return (int)usage;
}

int MemoryInfo::getAvailableVirtualMemory()
{
    return (int)(memoryStatus.ullAvailVirtual / COEFFICIENT_OF_DIVISION_MB);
}

int MemoryInfo::getTotalPagingFile()
{
    return (int)(memoryStatus.ullTotalPageFile / COEFFICIENT_OF_DIVISION_MB);
}

int MemoryInfo::getPagingFileUsage()
{
    double total = getTotalPagingFile();
    double available = getAvailablePagingFile();
    double usage = (total - available) / total * 100;
    return (int)usage;
}

int MemoryInfo::getAvailablePagingFile()
{
    return (int)(memoryStatus.ullAvailPageFile / COEFFICIENT_OF_DIVISION_MB);
}

HANDLE MemoryInfo::getProcessHandle(int pid)
{
    HANDLE hProcess = NULL;
    hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, 0, pid);

    return hProcess;
}

int MemoryInfo::getProcessPhysicalMemoryUsage(HANDLE hProcess)
{
    if(!hProcess) return -1;

    GetProcessMemoryInfo(hProcess, &processMemory, sizeof(processMemory));
    CloseHandle(hProcess);
    return (int)(processMemory.WorkingSetSize / COEFFICIENT_OF_DIVISION_MB);
}

int MemoryInfo::getProcessPeakPhysicalMemoryUsage(HANDLE hProcess)
{
    if(!hProcess) return -1;

    GetProcessMemoryInfo(hProcess, &processMemory, sizeof(processMemory));
    CloseHandle(hProcess);
    return (int)(processMemory.PeakWorkingSetSize / COEFFICIENT_OF_DIVISION_MB);
}

int MemoryInfo::getProcessPagingFileUsage(HANDLE hProcess)
{
    if(!hProcess) return -1;

    GetProcessMemoryInfo(hProcess, &processMemory, sizeof(processMemory));
    CloseHandle(hProcess);
    return (int)(processMemory.PagefileUsage / COEFFICIENT_OF_DIVISION_MB);
}

int MemoryInfo::getProcessPeakPagingFileUsage(HANDLE hProcess)
{
    if(!hProcess) return -1;

    GetProcessMemoryInfo(hProcess, &processMemory, sizeof(processMemory));
    CloseHandle(hProcess);
    return (int)(processMemory.PeakPagefileUsage / COEFFICIENT_OF_DIVISION_MB);
}
