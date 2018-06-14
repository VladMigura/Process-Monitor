#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

#include <windows.h>

class SystemInfo
{
    static const int BUFFER_SIZE = 128;
public:
    char processorName[BUFFER_SIZE];
    char processorID[BUFFER_SIZE];
    char processorVendorID[BUFFER_SIZE];
    char fpuID[BUFFER_SIZE];
    char motherBoardVendor[BUFFER_SIZE];
    char motherBoardModel[BUFFER_SIZE];
    char biosVersion[BUFFER_SIZE];
    char biosReleaseDate[BUFFER_SIZE];

    SystemInfo();

    void getSystemInfo();
};

#endif // SYSTEMINFO_H
