#pragma once

//winsock 2.2
#include <winsock2.h>

//TCP/IP specific extensions in Windows Sockets 2
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

// https://blog.csdn.net/hzrandd/article/details/50578373
// Bandwidth Delay Product ´ø¿íÊ±ÑÓ»ý
const long long BDP = 640000;//BDP = link_bandwidth * RTT
const int FPL = 256;

class CBlockingSocket
{
private:
	SOCKET m_socket;
	struct addrinfo *m_result;

public:
	CBlockingSocket();
	CBlockingSocket(SOCKET socket);
	virtual ~CBlockingSocket();
	static int Initialize();
	int HintsAndResult(const char *ip, const char *port);
	BOOL Open();
	BOOL Bind();
	BOOL Listen();
	CBlockingSocket Accept(struct sockaddr &their_addr);
	BOOL Connect();
	BOOL Send(const char *sendbuf, long long sendbuflen);
	int Read(char *recvbuf, int recvbuflen);
	BOOL Close();
	static void Cleanup();
	int getSendRecvBuffer() const;
	int setSendRecvBuffer(const int nBufLen);
	//struct in_addr getIpAddress();
};
