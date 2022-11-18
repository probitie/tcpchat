
#define _WINSOCK_DEPRECATED_NO_WARNINGS

// connect winsock static library
#pragma comment(lib, "ws2_32.lib")

// winsock header
#include <winsock2.h>

#include <iostream>
#include "logger.h"
#include <sstream>

SOCKET connections[100];
int currentConnectionsSize = 0;

void clientHandler(int index)
{
	int msg_size;

	while (true)
	{
		recv(connections[index], (char*)&msg_size, sizeof(int), NULL);
		char* msgl = new char[msg_size + 1];
		msgl[msg_size] = '\0';


		recv(connections[index], msgl, msg_size, NULL);
		debuglog("received message from client " << index << "; msg: " << msgl);
		// s << "client " << index << ": " << msg;
		//std::stringstream s;
		//s << "user " << index << " : ";
		//std::string&& ss = s.str();
		//strncpy_s(msgl, msg_size, ss.c_str(), ss.size());
		

		for(int i = 0; i < currentConnectionsSize; i++)
		{
			if (i == index)
				continue;
			debuglog("client to send is " << i << " max is " << currentConnectionsSize);
			debuglog("sending message size " << msg_size);
			send(connections[index], (char*)&msg_size, sizeof(int), NULL);

			debuglog("sending message context " << msgl);
			send(connections[i], msgl, msg_size, NULL);
		}
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

	for (int i = 0; i < 100; i++) // 100  is how much connections do we want
	{
		//                               client IP
		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrsize);

		// check if client successfully connected to the server
		if (newConnection == 0)
		{
			errorlog("client " << i << " can not connect to the server");
		}
		else
		{
			successlog("client " << i << " connected to the server");

			// send some data as a responce
			/*std::string message = "user is connected";
			int msg_size = message.size();
			send(newConnection, (char*)&msg_size, sizeof(int), NULL);
			send(newConnection, message.c_str(), msg_size, NULL);
			*/
			connections[i] = newConnection;
			currentConnectionsSize++;
			CreateThread(NULL, NULL,
				(LPTHREAD_START_ROUTINE)clientHandler,
				(LPWORD)(i), NULL, NULL);
		}
	}

	return 0;
}