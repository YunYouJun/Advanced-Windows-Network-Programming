#pragma once

//winsock 2.2
#include <winsock2.h>

//TCP/IP specific extensions in Windows Sockets 2
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

class CBlockingSocket
{
public:
	SOCKET m_socket;
	string m_ip;
	short m_port;
	struct sockaddr_in m_sin;

	CBlockingSocket();
	CBlockingSocket(SOCKET socket);
	CBlockingSocket(const char *ip, u_short port);
	static int Initialize();
	BOOL Open();
	BOOL Bind();
	BOOL Listen();
	CBlockingSocket Accept(struct sockaddr &their_addr);
	BOOL Connect();
	BOOL Send(const char *sendbuf, long long sendbuflen);
	int Recv(char *recvbuf, int recvbuflen);
	BOOL Close();
	static void Cleanup();
	int getSendRecvBuffer() const;
	int setSendRecvBuffer(const int nBufLen);
};
