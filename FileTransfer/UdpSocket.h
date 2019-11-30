#pragma once
//winsock 2.2
#include <winsock2.h>

//TCP/IP specific extensions in Windows Sockets 2
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

class UdpSocket
{
public:
	SOCKET m_socket;
	sockaddr_in m_addr;
	string m_ip;
	short m_port;
public:
	BOOL Open();
	BOOL Bind(const char *ip, u_short port);
	//https://docs.microsoft.com/zh-cn/windows/win32/api/winsock/nf-winsock-sendto
	//int sendto(
	//	SOCKET         s,
	//	const char     *buf,
	//	int            len,
	//	int            flags,
	//	const sockaddr *to,
	//	int            tolen
	//);
	// https://docs.microsoft.com/zh-cn/windows/win32/api/winsock/nf-winsock-recvfrom?redirectedfrom=MSDN
	//int recvfrom(
	//	SOCKET   s,
	//	char     *buf,
	//	int      len,
	//	int      flags,
	//	sockaddr *from,
	//	int      *fromlen
	//);
    void Select();
	BOOL Close();
};
