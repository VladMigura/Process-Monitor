#include "cpuinfo.h"

CPUInfo::CPUInfo()
{
    GetSystemTimes(&previousIdleTime, &previousKernelTime, &previousUserTime);
}

int CPUInfo::getTotalUsage()
{
    if(!GetSystemTimes(&idleTime, &kernelTime, &userTime)) return -1;

    _ULARGE_INTEGER pIdle;
    _ULARGE_INTEGER pKernel;
    _ULARGE_INTEGER pUser;
    _ULARGE_INTEGER idle;
    _ULARGE_INTEGER kernel;
    _ULARGE_INTEGER user;
    _ULARGE_INTEGER sys;
    _ULARGE_INTEGER totalUsage;

    idle.HighPart = idleTime.dwHighDateTime;
    idle.LowPart = idleTime.dwLowDateTime;

    pIdle.HighPart = previousIdleTime.dwHighDateTime;
    pIdle.LowPart = previousIdleTime.dwLowDateTime;

    kernel.HighPart = kernelTime.dwHighDateTime;
    kernel.LowPart = kernelTime.dwLowDateTime;

    pKernel.HighPart = previousKernelTime.dwHighDateTime;
    pKernel.LowPart = previousKernelTime.dwLowDateTime;

    user.HighPart = userTime.dwHighDateTime;
    user.LowPart = userTime.dwLowDateTime;

    pUser.HighPart = previousUserTime.dwHighDateTime;
    pUser.LowPart = previousUserTime.dwLowDateTime;

    idle.QuadPart = idle.QuadPart - pIdle.QuadPart;
    kernel.QuadPart = kernel.QuadPart - pKernel.QuadPart;
    user.QuadPart = user.QuadPart - pUser.QuadPart;

    sys.QuadPart = kernel.QuadPart + user.QuadPart;
    totalUsage.QuadPart = (sys.QuadPart - idle.QuadPart) * 100 / sys.QuadPart;

    previousIdleTime = idleTime;
    previousKernelTime = kernelTime;
    previousUserTime = userTime;

    return (int)totalUsage.QuadPart;
}

QString CPUInfo::getCPUFrequency()
{
    HKEY hKey;
    TCHAR lpData[2] = {0};
    DWORD bufferSize = sizeof(lpData);
    int frequency;

    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"), 0, KEY_READ, &hKey) != ERROR_SUCCESS) return "N/A";
    else
    {
        if(RegQueryValueEx(hKey, TEXT("~MHz"), NULL, NULL, (LPBYTE)lpData, &bufferSize) != ERROR_SUCCESS) return "N/A";
    }
    RegCloseKey(hKey);

    frequency = lpData[0];
    return QString::number(frequency);
}

QString CPUInfo::getCPUFeatures()
{
    QString output = "";
    int registers[4];

    __cpuid_count(0, 0, registers[0], registers[1], registers[2], registers[3]);
    int nIds = registers[0];
    __cpuid_count(0x80000000, 0, registers[0], registers[1], registers[2], registers[3]);
    unsigned nExIds = registers[0];

    if(nIds >= 0x00000001)
    {
        __cpuid_count(0x00000001, 0, registers[0], registers[1], registers[2], registers[3]);

        apic = (registers[3] & ((int)1 << 9)) != 0;
        acpi = (registers[3] & ((int)1 << 22)) != 0;
        htt = (registers[3] & ((int)1 << 28)) != 0;

        instructions.MSR = (registers[3] & ((int)1 << 5)) != 0;
        instructions.CX8 = (registers[3] & ((int)1 << 8)) != 0;
        instructions.SEP = (registers[3] & ((int)1 << 11)) != 0;
        instructions.FCMOV = (registers[3] & ((int)1 << 15)) != 0;
        instructions.CLFSH = (registers[3] & ((int)1 << 19)) != 0;
        instructions.MMX = (registers[3] & ((int)1 << 23)) != 0;
        instructions.FXSR = (registers[3] & ((int)1 << 24)) != 0;
        instructions.SSE = (registers[3] & ((int)1 << 25)) != 0;
        instructions.SSE2 = (registers[3] & ((int)1 << 26)) != 0;
        instructions.IA64 = (registers[3] & ((int)1 << 30)) != 0;

        instructions.SSE3 = (registers[2] & ((int)1 << 0)) != 0;
        instructions.PCLMULQDQ = (registers[2] & ((int)1 << 1)) != 0;
        instructions.MONITOR = (registers[2] & ((int)1 << 3)) != 0;
        instructions.SSSE3 = (registers[2] & ((int)1 << 9)) != 0;
        instructions.FMA = (registers[2] & ((int)1 << 12)) != 0;
        instructions.CMPXCHG16B = (registers[2] & ((int)1 << 13)) != 0;
        instructions.SSE41 = (registers[2] & ((int)1 << 19)) != 0;
        instructions.SSE42 = (registers[2] & ((int)1 << 20)) != 0;
        instructions.MOVBE = (registers[2] & ((int)1 << 22)) != 0;
        instructions.POPCNT = (registers[2] & ((int)1 << 23)) != 0;
        instructions.AES = (registers[2] & ((int)1 << 25)) != 0;
        instructions.XSAVE = (registers[2] & ((int)1 << 26)) != 0;
        instructions.OSXSAVE = (registers[2] & ((int)1 << 27)) != 0;
        instructions.AVX = (registers[2] & ((int)1 << 28)) != 0;
        instructions.F16C = (registers[2] & ((int)1 << 29)) != 0;
        instructions.RDRAND = (registers[2] & ((int)1 << 30)) != 0;
    }

    if(nIds >= 0x00000007)
    {
        __cpuid_count(0x00000007, 0, registers[0], registers[1], registers[2], registers[3]);

        instructions.FSGSBASE = (registers[1] & ((int)1 << 0)) != 0;
        instructions.SGX = (registers[1] & ((int)1 << 2)) != 0;
        instructions.BMI1 = (registers[1] & ((int)1 << 3)) != 0;
        instructions.HLE = (registers[1] & ((int)1 << 4)) != 0;
        instructions.AVX2 = (registers[1] & ((int)1 << 5)) != 0;
        instructions.BMI2 = (registers[1] & ((int)1 << 8)) != 0;
        instructions.ERMS = (registers[1] & ((int)1 << 9)) != 0;
        instructions.INVPCID = (registers[1] & ((int)1 << 10)) != 0;
        instructions.RTM = (registers[1] & ((int)1 << 11)) != 0;
        instructions.MPX = (registers[1] & ((int)1 << 14)) != 0;
        instructions.AVX512F = (registers[1] & ((int)1 << 16)) != 0;
        instructions.AVX512DQ = (registers[1] & ((int)1 << 17)) != 0;
        instructions.RDSEED = (registers[1] & ((int)1 << 18)) != 0;
        instructions.ADX = (registers[1] & ((int)1 << 19)) != 0;
        instructions.AVX512IFMA = (registers[1] &  ((int)1 << 21)) != 0;
        instructions.PCOMMIT = (registers[1] &  ((int)1 << 22)) != 0;
        instructions.CLFLUSHOPT = (registers[1] &  ((int)1 << 23)) != 0;
        instructions.CLWB = (registers[1] &  ((int)1 << 24)) != 0;
        instructions.AVX512PF = (registers[1] &  ((int)1 << 26)) != 0;
        instructions.AVX512ER = (registers[1] &  ((int)1 << 27)) != 0;
        instructions.AVX512CD = (registers[1] &  ((int)1 << 28)) != 0;
        instructions.SHA = (registers[1] &  ((int)1 << 29)) != 0;
        instructions.AVX512BW = (registers[1] &  ((int)1 << 30)) != 0;
        instructions.AVX512VL = (registers[1] &  ((int)1 << 31)) != 0;

        instructions.PREFETCHWT1 = (registers[2] & ((int)1 << 0)) != 0;
        instructions.AVX512VBMI = (registers[2] & ((int)1 << 1)) != 0;
        instructions.AVX512VBMI2 = (registers[2] & ((int)1 << 6)) != 0;
        instructions.GFNI = (registers[2] & ((int)1 << 8)) != 0;
        instructions.VAES = (registers[2] & ((int)1 << 9)) != 0;
        instructions.VPCLMULQDQ = (registers[2] & ((int)1 << 10)) != 0;
        instructions.AVX512VNNI = (registers[2] & ((int)1 << 11)) != 0;
        instructions.AVX512BITALG = (registers[2] & ((int)1 << 12)) != 0;
        instructions.AVX512VPOPCNTDQ = (registers[2] & ((int)1 << 14)) != 0;
    }

    if(nExIds >= 0x80000001)
    {
        __cpuid_count(0x80000001, 0, registers[0], registers[1], registers[2], registers[3]);

        instructions.LAHF = (registers[3] & ((int)1 << 0)) != 0;
        instructions.LZCNT = (registers[2] & ((int)1 << 5)) != 0;
        instructions.SYSCALL = (registers[3] & ((int)1 << 11)) != 0;
        instructions.MMXEXT = (registers[3] & ((int)1 << 22)) != 0;
        instructions.RDTSCP = (registers[3] & ((int)1 << 27)) != 0;
        instructions._3DNOWEXT = (registers[3] & ((int)1 << 30)) != 0;
        instructions._3DNOW = (registers[3] & ((int)1 << 31)) != 0;

        instructions.ABM = (registers[2] & ((int)1 << 5)) != 0;
        instructions.SSE4A = (registers[2] & ((int)1 << 6)) != 0;
        instructions.XOP = (registers[2] & ((int)1 << 11)) != 0;
        instructions.SKINIT = (registers[2] & ((int)1 << 12)) != 0;
        instructions.FMA4 = (registers[2] & ((int)1 << 16)) != 0;
        instructions.TBM = (registers[2] & ((int)1 << 21)) != 0;
    }

    output += "3DNOW\t\t" + checkInstruction(instructions._3DNOW) + "\n";
    output += "-------------------------------------------------------\n";
    output += "3DNOW Extensions\t" + checkInstruction(instructions._3DNOWEXT) + "\n";
    output += "-------------------------------------------------------\n";
    output += "ABM\t\t" + checkInstruction(instructions.ABM) + "\n";
    output += "-------------------------------------------------------\n";
    output += "Intel ADX\t\t" + checkInstruction(instructions.ADX) + "\n";
    output += "-------------------------------------------------------\n";
    output += "AES\t\t" + checkInstruction(instructions.AES) + "\n";
    output += "-------------------------------------------------------\n";
    output += "AVX\t\t" + checkInstruction(instructions.AVX) + "\n";
    output += "-------------------------------------------------------\n";
    output += "AVX2\t\t" + checkInstruction(instructions.AVX2) + "\n";
    output += "-------------------------------------------------------\n";
    output += "AVX512 BITALG\t\t" + checkInstruction(instructions.AVX512BITALG) + "\n";
    output += "-------------------------------------------------------\n";
    output += "AVX512BW\t\t" + checkInstruction(instructions.AVX512BW) + "\n";
    output += "-------------------------------------------------------\n";
    output += "AVX512CD\t\t" + checkInstruction(instructions.AVX512CD) + "\n";
    output += "-------------------------------------------------------\n";
    output += "AVX512DQ\t\t" + checkInstruction(instructions.AVX512DQ) + "\n";
    output += "-------------------------------------------------------\n";
    output += "AVX512ER\t\t" + checkInstruction(instructions.AVX512ER) + "\n";
    output += "-------------------------------------------------------\n";
    output += "AVX512F\t\t" + checkInstruction(instructions.AVX512F) + "\n";
    output += "-------------------------------------------------------\n";
    output += "AVX512IFMA\t\t" + checkInstruction(instructions.AVX512IFMA) + "\n";
    output += "-------------------------------------------------------\n";
    output += "AVX512PF\t\t" + checkInstruction(instructions.AVX512PF) + "\n";
    output += "-------------------------------------------------------\n";
    output += "AVX512VBMI\t\t" + checkInstruction(instructions.AVX512VBMI) + "\n";
    output += "-------------------------------------------------------\n";
    output += "AVX512VBMI2\t\t" + checkInstruction(instructions.AVX512VBMI2) + "\n";
    output += "-------------------------------------------------------\n";
    output += "AVX512VL\t\t" + checkInstruction(instructions.AVX512VL) + "\n";
    output += "-------------------------------------------------------\n";
    output += "AVX512VNNI\t\t" + checkInstruction(instructions.AVX512VNNI) + "\n";
    output += "-------------------------------------------------------\n";
    output += "AVX512VPOPCNTDQ\t" + checkInstruction(instructions.AVX512VPOPCNTDQ) + "\n";
    output += "-------------------------------------------------------\n";
    output += "BMI1\t\t" + checkInstruction(instructions.BMI1) + "\n";
    output += "-------------------------------------------------------\n";
    output += "BMI2\t\t" + checkInstruction(instructions.BMI2) + "\n";
    output += "-------------------------------------------------------\n";
    output += "CLFLUSHOPT\t\t" + checkInstruction(instructions.CLFLUSHOPT) + "\n";
    output += "-------------------------------------------------------\n";
    output += "CLFSH\t\t" + checkInstruction(instructions.CLFSH) + "\n";
    output += "-------------------------------------------------------\n";
    output += "CLWB\t\t" + checkInstruction(instructions.CLWB) + "\n";
    output += "-------------------------------------------------------\n";
    output += "CMPXCHG16B\t\t" + checkInstruction(instructions.CMPXCHG16B) + "\n";
    output += "-------------------------------------------------------\n";
    output += "CX8\t\t" + checkInstruction(instructions.CX8) + "\n";
    output += "-------------------------------------------------------\n";
    output += "ERMS\t\t" + checkInstruction(instructions.ERMS) + "\n";
    output += "-----------------------------------------------------\n";
    output += "F16C\t\t" + checkInstruction(instructions.F16C) + "\n";
    output += "-----------------------------------------------------\n";
    output += "FCMOV\t\t" + checkInstruction(instructions.FCMOV) + "\n";
    output += "-----------------------------------------------------\n";
    output += "FMA\t\t" + checkInstruction(instructions.FMA) + "\n";
    output += "-----------------------------------------------------\n";
    output += "FMA4\t\t" + checkInstruction(instructions.FMA4) + "\n";
    output += "-----------------------------------------------------\n";
    output += "FSGSBASE\t\t" + checkInstruction(instructions.FSGSBASE) + "\n";
    output += "-----------------------------------------------------\n";
    output += "FXSR\t\t" + checkInstruction(instructions.FXSR) + "\n";
    output += "-----------------------------------------------------\n";
    output += "GFNI\t\t" + checkInstruction(instructions.GFNI) + "\n";
    output += "-----------------------------------------------------\n";
    output += "HLE\t\t" + checkInstruction(instructions.HLE) + "\n";
    output += "-----------------------------------------------------\n";
    output += "IA-64\t\t" + checkInstruction(instructions.IA64) + "\n";
    output += "-----------------------------------------------------\n";
    output += "INVPCID\t\t" + checkInstruction(instructions.INVPCID) + "\n";
    output += "-----------------------------------------------------\n";
    output += "LAHF\t\t" + checkInstruction(instructions.LAHF) + "\n";
    output += "-----------------------------------------------------\n";
    output += "LZCNT\t\t" + checkInstruction(instructions.LZCNT) + "\n";
    output += "-----------------------------------------------------\n";
    output += "MMX\t\t" + checkInstruction(instructions.MMX) + "\n";
    output += "-----------------------------------------------------\n";
    output += "MMX Extended\t\t" + checkInstruction(instructions.MMXEXT) + "\n";
    output += "-----------------------------------------------------\n";
    output += "MONITOR\t\t" + checkInstruction(instructions.MONITOR) + "\n";
    output += "-----------------------------------------------------\n";
    output += "MOVBE\t\t" + checkInstruction(instructions.MOVBE) + "\n";
    output += "-----------------------------------------------------\n";
    output += "Intel MPX\t\t" + checkInstruction(instructions.MPX) + "\n";
    output += "-----------------------------------------------------\n";
    output += "MSR\t\t" + checkInstruction(instructions.MSR) + "\n";
    output += "-----------------------------------------------------\n";
    output += "OSXSAVE\t\t" + checkInstruction(instructions.OSXSAVE) + "\n";
    output += "-----------------------------------------------------\n";
    output += "PCLMULQDQ\t\t" + checkInstruction(instructions.PCLMULQDQ) + "\n";
    output += "-----------------------------------------------------\n";
    output += "PCOMMIT\t\t" + checkInstruction(instructions.PCOMMIT) + "\n";
    output += "-----------------------------------------------------\n";
    output += "POPCNT\t\t" + checkInstruction(instructions.POPCNT) + "\n";
    output += "-----------------------------------------------------\n";
    output += "PREFETCHWT1\t\t" + checkInstruction(instructions.PREFETCHWT1) + "\n";
    output += "-----------------------------------------------------\n";
    output += "RDRAND\t\t" + checkInstruction(instructions.RDRAND) + "\n";
    output += "-----------------------------------------------------\n";
    output += "RDSEED\t\t" + checkInstruction(instructions.RDSEED) + "\n";
    output += "-----------------------------------------------------\n";
    output += "RDTSCP\t\t" + checkInstruction(instructions.RDTSCP) + "\n";
    output += "-----------------------------------------------------\n";
    output += "RTM\t\t" + checkInstruction(instructions.RTM) + "\n";
    output += "-----------------------------------------------------\n";
    output += "SEP\t\t" + checkInstruction(instructions.SEP) + "\n";
    output += "-----------------------------------------------------\n";
    output += "SGX\t\t" + checkInstruction(instructions.SGX) + "\n";
    output += "-----------------------------------------------------\n";
    output += "Intel SHA Extensions\t" + checkInstruction(instructions.SHA) + "\n";
    output += "-----------------------------------------------------\n";
    output += "SKINIT\t\t" + checkInstruction(instructions.SKINIT) + "\n";
    output += "-----------------------------------------------------\n";
    output += "SSE\t\t" + checkInstruction(instructions.SSE) + "\n";
    output += "-----------------------------------------------------\n";
    output += "SSE2\t\t" + checkInstruction(instructions.SSE2) + "\n";
    output += "-----------------------------------------------------\n";
    output += "SSE3\t\t" + checkInstruction(instructions.SSE3) + "\n";
    output += "-----------------------------------------------------\n";
    output += "SSE4.1\t\t" + checkInstruction(instructions.SSE41) + "\n";
    output += "-----------------------------------------------------\n";
    output += "SSE4.2\t\t" + checkInstruction(instructions.SSE42) + "\n";
    output += "-----------------------------------------------------\n";
    output += "SSE4A\t\t" + checkInstruction(instructions.SSE4A) + "\n";
    output += "-----------------------------------------------------\n";
    output += "SSSE3\t\t" + checkInstruction(instructions.SSSE3) + "\n";
    output += "-----------------------------------------------------\n";
    output += "SYSCALL\t\t" + checkInstruction(instructions.SYSCALL) + "\n";
    output += "-----------------------------------------------------\n";
    output += "TBM\t\t" + checkInstruction(instructions.TBM) + "\n";
    output += "-----------------------------------------------------\n";
    output += "VAES\t\t" + checkInstruction(instructions.VAES) + "\n";
    output += "-----------------------------------------------------\n";
    output += "VPCLMULQDQ\t\t" + checkInstruction(instructions.VPCLMULQDQ) + "\n";
    output += "-----------------------------------------------------\n";
    output += "XOP\t\t" + checkInstruction(instructions.XOP) + "\n";
    output += "-----------------------------------------------------\n";
    output += "XSAVE\t\t" + checkInstruction(instructions.XSAVE) + "\n";
    output += "-----------------------------------------------------\n";

    return output;
}

void CPUInfo::getCacheInfo(QString cpuVendor)
{
    int registers[4];

    if(cpuVendor == "GenuineIntel")
    {
        int ways;
        int partitions;
        int lineSize;
        int sets;

        for(int i = 0; i < 4; i++)
        {
            __cpuid_count(0x00000004, i, registers[0], registers[1], registers[2], registers[3]);
            ways = registers[1] & 0xffc00000;
            ways >>=22;
            ways++;
            partitions = registers[1] & 0x3ff800;
            partitions >>= 22;
            partitions++;
            lineSize = registers[1] & 0x7ff;
            lineSize++;
            sets = registers[2];
            sets++;

            switch(i)
            {
                case 0:
                    cacheL1DataInfo = QString::number(ways * partitions * lineSize * sets / 1024) + " KB ";
                    cacheL1DataInfo += QString::number(ways) + "-way";
                    break;

                case 1:
                    cacheL1InstructionsInfo = QString::number(ways * partitions * lineSize * sets / 1024) + " KB ";
                    cacheL1InstructionsInfo += QString::number(ways) + "-way";
                    break;

                case 2:
                    cacheL2Info = QString::number(ways * partitions * lineSize * sets / 1024) + " KB ";
                    cacheL2Info += QString::number(ways) + "-way";
                    break;

                case 3:
                    cacheL3Info = QString::number(ways * partitions * lineSize * sets / 1024) + " KB ";
                    cacheL3Info += QString::number(ways) + "-way";
                    break;
            }

        }

    }
    else
    {
        __cpuid_count(0x80000005, 0, registers[0], registers[1], registers[2], registers[3]);
        cacheL1InstructionsInfo = QString::number((registers[3] >> 24) & 0xff) + " KB";
        cacheL1DataInfo = QString::number((registers[2] >> 24) & 0xff) + "KB";

        __cpuid_count(0x80000006, 0, registers[0], registers[1], registers[2], registers[3]);
        cacheL2Info = QString::number((registers[2] >> 16) & 0xffff) + " KB";

        __cpuid_count(0x80000006, 0, registers[0], registers[1], registers[2], registers[3]);
        cacheL3Info = QString::number((registers[3] >> 18) & 0xffff) + " KB";
    }
}

int CPUInfo::getNumOfProcessors()
{
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    return sysInfo.dwNumberOfProcessors;
}

QString CPUInfo::checkInstruction(bool instruction)
{
    if(instruction) return "supported";
    else return "not supported";
}
