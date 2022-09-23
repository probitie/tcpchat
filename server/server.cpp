
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
	successlog(" WinSock2.1 lib has been loaded");

	// reserve an internet socket address
	SOCKADDR_IN addr;
	int addrsize = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // set ip as localhost
	addr.sin_port = htons(9999); // set a port
	addr.sin_family = AF_INET; // internet protocol family



	///////////  SERVER STUFF  ///////////


	// create an actual socket
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);

	// connect the socket with its address
	bind(sListen, (SOCKADDR*)&addr, addrsize);

	// start listening the port with this socket
	//  the socket   |   max connections to the socket
	listen(sListen, SOMAXCONN);

	// when someone requests the server by that socket we start a new connection

	SOCKET newConnection;

	//                               client IP
	newConnection = accept(sListen, (SOCKADDR*)&addr, &addrsize);

	// check if client successfully connected to the server
	if (newConnection == 0)
	{
		errorlog("client can not connect to the server");
	}

	successlog("client connected to the server");
		
	// send some data as a responce
	char message[256] = "hello there, you've just got a responce from the server";
	send(newConnection, message, sizeof(message), NULL);


	system("pause");
	return 0;
}