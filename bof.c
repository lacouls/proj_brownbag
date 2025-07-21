//BOF Creation

#include <stdio.h>
#include <windows.h>
#include "beacon.h"

WINBASEAPI BOOL WINAPI KERNEL32$CREATEPROCESSW(LPCWSTR lpApplicationName,LPWSTR lpCommandLine,LPSECURITY_ATTRIBUTES lpProcessAttributes,LPSECURITY_ATTRIBUTES lpThreadAttributes,BOOL bInheritHandles,DWORD dwCreationFlags,LPVOID lpEnvironment,LPCWSTR lpCurrentDirectory,LPSTARTUPINFOW lpStartupInfo,LPPROCESS_INFORMATION lpProcessInformation);

void go(char * buff, int len){
	
	STARTUPINFOW si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	
	KERNEL32$CREATEPROCESSW(L"C:\\Windows\\system32\\calc.exe", NULL, NULL, NULL, FALSE, BELOW_NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	
	return 0; 
	
}
