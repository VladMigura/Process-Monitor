#ifndef CPUINFO_H
#define CPUINFO_H

#include <QString>
#include <windows.h>
#include <intrin.h>
#include <cpuid.h>

struct Instructions
{
    bool _3DNOW;
    bool _3DNOWEXT;
    bool ABM;
    bool ADX;
    bool AES;
    bool AVX;
    bool AVX2;
    bool AVX512BITALG;
    bool AVX512BW;
    bool AVX512CD;
    bool AVX512DQ;
    bool AVX512ER;
    bool AVX512F;
    bool AVX512IFMA;
    bool AVX512PF;
    bool AVX512VBMI;
    bool AVX512VBMI2;
    bool AVX512VL;
    bool AVX512VNNI;
    bool AVX512VPOPCNTDQ;
    bool BMI1;
    bool BMI2;
    bool CLFLUSHOPT;
    bool CLFSH;
    bool CLWB;
    bool CMPXCHG16B;
    bool CX8;
    bool ERMS;
    bool F16C;
    bool FCMOV;
    bool FMA;
    bool FMA4;
    bool FSGSBASE;
    bool FXSR;
    bool GFNI;
    bool HLE;
    bool IA64;
    bool INVPCID;
    bool LAHF;
    bool LZCNT;
    bool MMX;
    bool MMXEXT;
    bool MONITOR;
    bool MOVBE;
    bool MPX;
    bool MSR;
    bool OSXSAVE;
    bool PCLMULQDQ;
    bool PCOMMIT;
    bool POPCNT;
    bool PREFETCHWT1;
    bool RDRAND;
    bool RDSEED;
    bool RDTSCP;
    bool RTM;
    bool SEP;
    bool SGX;
    bool SHA;
    bool SKINIT;
    bool SSE;
    bool SSE2;
    bool SSE3;
    bool SSE41;
    bool SSE42;
    bool SSE4A;
    bool SSSE3;
    bool SYSCALL;
    bool TBM;
    bool VAES;
    bool VPCLMULQDQ;
    bool XOP;
    bool XSAVE;
};

class CPUInfo
{   
    FILETIME previousIdleTime;
    FILETIME previousKernelTime;
    FILETIME previousUserTime;
    FILETIME idleTime;
    FILETIME kernelTime;
    FILETIME userTime;

    Instructions instructions;

public:
    bool apic;
    bool acpi;
    bool htt;

    QString cacheL1InstructionsInfo;
    QString cacheL1DataInfo;
    QString cacheL2Info;
    QString cacheL3Info;

    CPUInfo();

    int getTotalUsage();
    QString getCPUFrequency();
    QString getCPUFeatures();
    void getCacheInfo(QString cpuVendor);
    int getNumOfProcessors();

    QString checkInstruction(bool instruction);
};

#endif // CPUINFO_H
