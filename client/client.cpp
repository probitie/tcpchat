#define _WINSOCK_DEPRECATED_NO_WARNINGS


// connect winsock static library
#pragma comment(lib, "ws2_32.lib")

// winsock header
#include <winsock2.h>

#include <iostream>
#include "logger.h"


int main(int argc, char* argv[])
{

	// load library
	WSAData wsaDate;

	WORD DLLVersion = MAKEWORD(2, 1); // lib version 2.1

	// if something went wrong with initializing
	if (WSAStartup(DLLVersion, &wsaDate) != 0)
	{
		errorlog("can not load WinSock2.1 lib");
		exit(-1);
	}
	successlog("WinSock2.1 lib has been loaded");

	// reserve an internet socket address
	SOCKADDR_IN addr;
	int addrsize = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // set ip as localhost
	addr.sin_port = htons(9999); // set a port
	addr.sin_family = AF_INET; // internet protocol family



	////////// CLIENT STUFF ////////
	
	// connect to the server
	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);

	// check if connection was successfull
	if (connect(Connection, (SOCKADDR*)&addr, addrsize))
	{
		errorlog("Error: can not connect to the server");
		return -1;
	}
	
	successlog("connected to the server");

	// receive server responce
	char responce[256]{};
	recv(Connection, responce, sizeof(responce), NULL);

	// process responce
	debuglog("oh, the server sent me smth: " << responce);


	system("pause");
	return 0;
}