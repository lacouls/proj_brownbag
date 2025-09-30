#include <stdio.h>
#include <windows.h>
#include <wchar.h>
#include <string.h>
#include "beacon.h"



//HEADER
WINBASEAPI HANDLE WINAPI KERNEL32$FindFirstFileW (LPCWSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData);
WINBASEAPI WINBOOL WINAPI KERNEL32$FindNextFileW (HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData);
WINBASEAPI DWORD WINAPI KERNEL32$GetLastError (VOID);
WINBASEAPI WINBOOL WINAPI KERNEL32$FindClose (HANDLE hFindFile);


void go(char * buff, int len){
    WIN32_FIND_DATAW lpFindFileData;
    HANDLE hFind = KERNEL32$FindFirstFileW(L".\\*", &lpFindFileData);
	BeaconPrintf(CALLBACK_OUTPUT,"DIRECTORY LISTING\n\n\n", lpFindFileData.cFileName);
	
    if (hFind == INVALID_HANDLE_VALUE) {
        BeaconPrintf(CALLBACK_ERROR,"FindFirstFile failed with error %lu\n", KERNEL32$GetLastError());
        //return 0;
    } else {
        do {
			BeaconPrintf(CALLBACK_OUTPUT,"%ls", &lpFindFileData.cFileName);
        } while (KERNEL32$FindNextFileW(hFind, &lpFindFileData) != 0);

        KERNEL32$FindClose(hFind);
    }
}

