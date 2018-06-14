#include "systeminfo.h"

SystemInfo::SystemInfo()
{

}

void SystemInfo::getSystemInfo()
{
    HKEY hKey;
    TCHAR lpData[255] = {0};
    DWORD bufferSize = sizeof(lpData);

    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"), NULL, KEY_READ, &hKey) != ERROR_SUCCESS)
    {
        strcpy(processorName, "N/A");
        strcpy(processorID, "N/A");
        strcpy(processorVendorID, "N/A");
    }
    else
    {
        if(RegQueryValueEx(hKey, TEXT("ProcessorNameString"), NULL, NULL, (LPBYTE)lpData, &bufferSize) != ERROR_SUCCESS) strcpy(processorName, "N/A");
        else WideCharToMultiByte(CP_ACP, 0, lpData, -1, processorName, BUFFER_SIZE, NULL, NULL);
        bufferSize = sizeof(lpData);
        memset(lpData, 0, bufferSize);

        if(RegQueryValueEx(hKey, TEXT("Identifier"), NULL, NULL, (LPBYTE)lpData, &bufferSize) != ERROR_SUCCESS) strcpy(processorID, "N/A");
        else WideCharToMultiByte(CP_ACP, 0, lpData, -1, processorID, BUFFER_SIZE, NULL, NULL);
        bufferSize = sizeof(lpData);
        memset(lpData, 0, bufferSize);

        if(RegQueryValueEx(hKey, TEXT("VendorIdentifier"), NULL, NULL, (LPBYTE)lpData, &bufferSize) != ERROR_SUCCESS) strcpy(processorVendorID, "N/A");
        else WideCharToMultiByte(CP_ACP, 0, lpData, -1, processorVendorID, BUFFER_SIZE, NULL, NULL);
        bufferSize = sizeof(lpData);
        memset(lpData, 0, bufferSize);
    }

    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DESCRIPTION\\System\\BIOS"), NULL, KEY_READ, &hKey) != ERROR_SUCCESS)
    {
        strcpy(motherBoardModel, "N/A");
        strcpy(motherBoardVendor, "N/A");
        strcpy(biosVersion, "N/A");
        strcpy(biosReleaseDate, "N/A");
    }
    else
    {
        if(RegQueryValueEx(hKey, TEXT("BaseBoardProduct"), NULL, NULL, (LPBYTE)lpData, &bufferSize) != ERROR_SUCCESS) strcpy(motherBoardModel, "N/A");
        else WideCharToMultiByte(CP_ACP, 0, lpData, -1, motherBoardModel, BUFFER_SIZE, NULL, NULL);
        bufferSize = sizeof(lpData);
        memset(lpData, 0, bufferSize);

        if(RegQueryValueEx(hKey, TEXT("BaseBoardManufacturer"), NULL, NULL, (LPBYTE)lpData, &bufferSize) != ERROR_SUCCESS) strcpy(motherBoardVendor, "N/A");
        else WideCharToMultiByte(CP_ACP, 0, lpData, -1, motherBoardVendor, BUFFER_SIZE, NULL, NULL);
        bufferSize = sizeof(lpData);
        memset(lpData, 0, bufferSize);

        if(RegQueryValueEx(hKey, TEXT("BIOSVersion"), NULL, NULL, (LPBYTE)lpData, &bufferSize) != ERROR_SUCCESS) strcpy(biosVersion, "N/A");
        else WideCharToMultiByte(CP_ACP, 0, lpData, -1, biosVersion, BUFFER_SIZE, NULL, NULL);
        bufferSize = sizeof(lpData);
        memset(lpData, 0, bufferSize);

        if(RegQueryValueEx(hKey, TEXT("BIOSReleaseDate"), NULL, NULL, (LPBYTE)lpData, &bufferSize) != ERROR_SUCCESS) strcpy(biosReleaseDate, "N/A");
        else WideCharToMultiByte(CP_ACP, 0, lpData, -1, biosReleaseDate, BUFFER_SIZE, NULL, NULL);
        bufferSize = sizeof(lpData);
        memset(lpData, 0, bufferSize);
    }

    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DESCRIPTION\\System\\FloatingPointProcessor\\0"), NULL, KEY_READ, &hKey) != ERROR_SUCCESS)
    {
        strcpy(fpuID, "N/A");
    }
    else
    {
        if(RegQueryValueEx(hKey, TEXT("Identifier"), NULL, NULL, (LPBYTE)lpData, &bufferSize) != ERROR_SUCCESS) strcpy(fpuID, "N/A");
        else WideCharToMultiByte(CP_ACP, 0, lpData, -1, fpuID, BUFFER_SIZE, NULL, NULL);
        bufferSize = sizeof(lpData);
        memset(lpData, 0, bufferSize);
    }

    RegCloseKey(hKey);
}
