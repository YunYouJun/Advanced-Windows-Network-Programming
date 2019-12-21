#pragma once
#include "URL.h"
#define TCP 1
#define UDP 0

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

class ClientSocket
{
public:
    ClientSocket();
    ~ClientSocket();

    SOCKET m_socket;

    char m_rtp_server_port[5];
    char m_rtp_server_ip[16];

    struct addrinfo* RTSPServerInfo;
    struct addrinfo* RTPServerInfo;
    struct addrinfo* RTPClientInfo;
    struct sockaddr_in ClientAddr;
    socklen_t addrLen;

    bool Open(int type);
    bool Bind(u_short port);
    bool Connect(const char *ip, u_short port);
    bool Send(const char*sendbuf, int sendbuflen);
	int Recv(char* recvbuf);
	int RecvFrom(char* recvbuf, const char* ip, u_short port);
    void Close();
};
