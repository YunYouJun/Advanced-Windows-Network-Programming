// EchoClient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "pch.h"
#include <winsock2.h>
#include <iostream>
#include <string>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main(int argc, char *argv[])
{
	string ip;
	string port = "8888";
	string message = "Hello World!";
	// 10.64.132.147
	if (argc == 1) {
		cout << "Please input target ip:" << endl;
		cin >> ip;
	}
	else if (argc == 3) {
		ip = argv[1];
		port = argv[2];
	}
	else if (argc == 4) {
		ip = argv[1];
		port = argv[2];
		message = argv[3];
	} else if (argc > 4) {
		cout << "EchoClient need params: ip, port, message(optional)." << endl;
		cout << "Please input like this: EchoClient 10.64.132.147 8888" << endl;
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
	SOCKET m_hConnectSocket;
	m_hConnectSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_hConnectSocket == INVALID_SOCKET)
	{
		printf("Socket failed with error %d\n", WSAGetLastError());
		return -1;
	}

	// pdf P57
	// Bind socket
	// Connecting
	struct sockaddr_in ServerAddr;
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr(ip.c_str());
	ServerAddr.sin_port = htons(atoi(port.c_str()));
	connect(m_hConnectSocket, (struct sockaddr *) &ServerAddr, sizeof(ServerAddr));
	cout << "Connection established to remote Server at " << ip << ":" << port << endl;

	// pdf P65
	// Sending data
	int send_bytes = send(m_hConnectSocket, message.c_str(), strlen(message.c_str()), 0);
	if (send_bytes < 0) {
		printf("SOCKET END ERROR: %d\n", GetLastError());
		return -1;
	}
	cout << "  Sent " << send_bytes << " bytes back to the server: " << message << endl;

	// pdf P63
	// Receiving data
	char buf[256] = { 0 };
	int recv_bytes = recv(m_hConnectSocket, buf, sizeof(buf), 0);
	cout << "  Received " << recv_bytes << " bytes from the server.";

	// judge
	if (strcmp(message.c_str(), buf) == 0) {
		cout << " Reply matches what we sent!" << endl;
	}
	else {
		cout << " Reply doesn't match what we sent!" << endl;
	}

	cout << "  Shutdown the connection in 10 seconds……" << endl;
	Sleep(10000);

	// disable new sends, receives, or both, respectively.
	shutdown(m_hConnectSocket, SD_BOTH);
	// to assure that all data is send and received on a connection, an application should call shutdown before calling closesocket
	if (closesocket(m_hConnectSocket) == 0) {
		cout << "Connection is down" << endl;
	}
	else {
		cout << "Close fail" << endl;
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
