#define _WINSOCK_DEPRECATED_NO_WARNINGS


// connect winsock static library
#pragma comment(lib, "ws2_32.lib")

// winsock header
#include <winsock2.h>

#include <iostream>
#include <string>
#include "logger.h"

SOCKET Connection;

void ClientHandler()
{
	int msg_size;
	while (true)
	{
		recv(Connection, (char*)&msg_size, sizeof(int), NULL);
		debuglog("received message, length " << msg_size);
		char* msgl = new char[100 + 1] {'\0'};
		//msgl[msg_size] = '\0';
		recv(Connection, msgl, msg_size, NULL);
		debuglog("received message, context" << msgl);
		std::cout << msgl << std::endl;
		delete[] msgl;
	}
}

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
	Connection = socket(AF_INET, SOCK_STREAM, NULL);

	// check if connection was successfull
	if (connect(Connection, (SOCKADDR*)&addr, addrsize))
	{
		errorlog("Error: can not connect to the server");
		return -1;
	}
	
	successlog("connected to the server");

	// receive server responce
	/*int resp_size;
	recv(Connection, (char*)&resp_size, sizeof(int), NULL);
	
	char* responce = new char[resp_size];

	recv(Connection, responce, resp_size, NULL);

	// process responce
	debuglog("oh, the server sent me smth: " << responce);
	delete[] responce;
	*/
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);
	std::string msgl;
	while (true)
	{
		getline(std::cin, msgl);
		int msg_size = msgl.size();
		send(Connection, (char*)&msg_size, sizeof(int), NULL);
		send(Connection, msgl.c_str(), msg_size, NULL);
		Sleep(10);
	}


	system("pause");
	return 0;
}