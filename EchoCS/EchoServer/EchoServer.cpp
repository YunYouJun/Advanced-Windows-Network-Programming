// EchoServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "pch.h"
#include <WinSock2.h>
#include <iostream>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

int main(int argc, char *argv[])
{
	string port;
	if (argc == 1) {
		cout << "EchoServer use default port: 8888" << endl;
		port = "8888";
	}
	else if (argc == 2) {
		port = argv[1];
	}
	else {
		cout << "EchoServer just need a port param." << endl;
		cout << "Like this: EchoServer 8888" << endl;
		return -1;
	}

	// pdf P48
	WORD wVersionRequested;
	wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	// Initialize Winsock version 2.2
	//将两个byte型合并成一个word型，一个在高8位(b)，一个在低8位(a)
	// 512 + 2 = 514
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		cout << "WSAStartup failed" << endl;
		return -1;
	}

	// pdf P55
	// Creating a Socket
	SOCKET m_hListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_hListenSocket == INVALID_SOCKET)
	{
		printf("Socket failed with error %d\n", WSAGetLastError());
		return -1;
	}

	// pdf P57
	// Bind socket
	struct sockaddr_in ServerAddr;
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddr.sin_port = htons(atoi(port.c_str()));
	bind(m_hListenSocket, (struct sockaddr *) &ServerAddr, sizeof(ServerAddr));

	// pdf P59
	// Listen to incoming requests
	// modify backlog
	// int listen(SOCKET s, int backlog)
	listen(m_hListenSocket, 3);
	
	char name[256];
	gethostname(name, sizeof(name));
	hostent *remoteHost = gethostbyname(name);
	struct in_addr addr;
	addr.s_addr = *(u_long *)remoteHost->h_addr_list[0];
	cout << "EchoServer is listening on " << inet_ntoa(addr) << ":" << port.c_str() << endl;

	// pdf P61
	// Accept Incoming Connection
	struct sockaddr_in ClientAddr;
	int Size = sizeof(ClientAddr);
	
	SOCKET m_hAcceptSocket;
	char message[256] = { 0 };
	while (1)
	{
		cout << "Waiting for a connection..." << endl;
		memset(message, 0, sizeof(message));
		m_hAcceptSocket = accept(m_hListenSocket, (SOCKADDR *)&ClientAddr, &Size);
		cout << "Accepted connection from " << inet_ntoa(ClientAddr.sin_addr) << endl;

		// pdf P63
		// Receiving data
		int recv_bytes = recv(m_hAcceptSocket, message, sizeof(message), 0);
		if (recv_bytes > 0)
		{
			cout << "  Received " << recv_bytes << " bytes from the client: " << message << endl;
		}
		else
		{
			cout << "Recieve fail" << endl;
			return -1;
		}

		// pdf P65
		// Sending data
		int send_bytes = send(m_hAcceptSocket, message, strlen(message), 0);
		if (send_bytes < 0) {
			printf("SOCKET END ERROR: %d\n", GetLastError());
			return -1;
		}
		else {
			cout << "  Sent " << send_bytes << " bytes back to the client: " << message << endl;
		}

		// wait client close
		while (recv(m_hAcceptSocket, message, sizeof(message), 0) != 0)
		{
			break;
		}
		// to assure that all data is send and received on a connection, an application should call shutdown before calling closesocket
		if (closesocket(m_hAcceptSocket) == 0)
		{
			cout << "Connection closed by the client" << endl;
		}
		else
		{
			cout << "Close fail" << endl;
		}
	}

	// pdf P56
	// Error Checking and Handling
	if (WSACleanup() == SOCKET_ERROR)
	{
		printf("WSACleanup failed with error %d\n", WSAGetLastError());
		return -1;
	}

	return 0;
}
