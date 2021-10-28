#include <iostream>
#include <windows.h>
#include <stdio.h>
#include<tchar.h>

using namespace std;

#define INFO_BUFFER_SIZE 32767
#define BUFSIZE MAX_PATH
#define MAX_KEY_LENGTH 255

double PCFreq=0;
        __int64 CounterStart = 0;


// 2.1. Замер рабочей частоты
        void StartCounter()
        {
            LARGE_INTEGER f;
            if(!QueryPerformanceFrequency(&f))
                printf("Function QueryPerformanceFrequency() failed \n");
            PCFreq = double(f.QuadPart);
            printf("\n CPU frequency %u Hz\n",f);

            QueryPerformanceCounter(&f);
            CounterStart = f.QuadPart;
        }

//2.2. Подсчёт количества тактов
        double GetCounter()
        {
            LARGE_INTEGER n;
            QueryPerformanceCounter(&n);
            return double ((n.QuadPart - CounterStart)*1000000)/PCFreq;
        }

int main()
{
    //1.1. Определение версии операционной системы

    OSVERSIONINFO osvi;
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);
    printf("Version OS: Windows %i,%i\n", osvi.dwMajorVersion, osvi.dwMinorVersion,osvi.dwBuildNumber);

    //1.2. Определение системного каталога

    DWORD i;
    TCHAR infoBuf[INFO_BUFFER_SIZE];
    DWORD bufCharCount = INFO_BUFFER_SIZE;
    GetSystemDirectory(infoBuf,INFO_BUFFER_SIZE);
    printf("System Directory: %s\n",infoBuf);

    //1.3. Определение названия компьютера и псевдонима текущего пользователя
    bufCharCount = INFO_BUFFER_SIZE;
    GetComputerName(infoBuf, &bufCharCount);
    printf("Computer name: %s\n", infoBuf);

    bufCharCount = INFO_BUFFER_SIZE;
    GetUserName(infoBuf, &bufCharCount);
    printf("User name: %s\n",infoBuf);

    //1.4. Определение данных томов

    char buffer[MAX_PATH + 1];
    DWORD CBufLen = MAX_PATH;
    CHAR Vol[MAX_PATH + 1];
    CHAR VolumeName[MAX_PATH];
    __int64 total, available, free;

    HANDLE search = FindFirstVolume(VolumeName,BUFSIZE);
    printf("\n    %s",VolumeName);

    do
        {

        printf("\n   %s",Vol);
        GetVolumePathNamesForVolumeName(Vol,buffer,CBufLen,&CBufLen);
        char* path = buffer;
        printf("\n   First path: %s",path);
        GetDiskFreeSpaceEx(buffer,(PULARGE_INTEGER)&available,(PULARGE_INTEGER)&total,(PULARGE_INTEGER)&free);
        if(GetDiskFreeSpaceEx(buffer,(PULARGE_INTEGER)&available,(PULARGE_INTEGER)&total,(PULARGE_INTEGER)&free)==0)
        {
            printf("\n   No information available\n");
        }
        else
        {
            printf("\n   Total size: %u bytes", total);
            printf("\n   Free space: %u bytes", available);
        }
        }
        while(FindNextVolume(search,Vol,BUFSIZE));
        FindVolumeClose(search);

        //1.5. Вывод списка программ

        DWORD dwSize;

        TCHAR szName[MAX_KEY_LENGTH];

        HKEY hKey;

        DWORD dwIndex = 0;

        DWORD retCode;

        DWORD BufferSize = 8192;

        DWORD cbData = BufferSize;

        PPERF_DATA_BLOCK PerfData = (PPERF_DATA_BLOCK)malloc(BufferSize);


        if (RegOpenKeyEx(HKEY_CURRENT_USER,

         "Software\\Microsoft\\Windows\\CurrentVersion\\Run",

         0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)

        {

          printf("\n Function RegOpenKeyEx() failed\n");

        }

        else printf("\n  Startup commands: \n");

        while (1)
            {

        dwSize = sizeof(szName);

        retCode = RegEnumValue(hKey, dwIndex, szName, &dwSize, NULL, NULL, NULL, NULL);

        if (retCode == ERROR_SUCCESS)

        {

          RegQueryValueEx(hKey, szName, NULL, NULL, (LPBYTE)PerfData, &cbData);

          printf("   %d) %s %s\n", dwIndex + 1, szName, PerfData);

          dwIndex++;

        }

        else break;

        }

        RegCloseKey(hKey);

        //Âûâîä ðåçóëüòàòà ïóíêòà 2

        StartCounter();
        cout<<" CPU clock count: "<<GetCounter()<<" us\n";
        return 0;
}
