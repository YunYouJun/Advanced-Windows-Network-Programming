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

    SOCKET m_tcp_socket;

    char m_rtp_server_port[5];
    char m_rtp_server_ip[16];

    SOCKET RTSPSocket, RTPSocket;
    SOCKET ConnectSocket;
    struct addrinfo* RTSPServerInfo;
    struct addrinfo* RTPServerInfo;
    struct addrinfo* RTPClientInfo;
    struct sockaddr_in ClientAddr;
    socklen_t addrLen;

    bool Initialize();
    bool Open();
    bool Bind(const char *ip, u_short port);
    bool Connect(SOCKET &Socket, struct addrinfo *serverInfo);
    bool Send(SOCKET &Socket, const char*sendbuf, int sendbuflen);
    char* Recv(SOCKET &Socket);
    int RecvRTP(SOCKET &Socket, char* recvbuf);
    int RecvFrom(SOCKET &Socket, struct addrinfo *serverInfo, char *recvBuf, int revBuflen);
    void Close(SOCKET &Socket);
};

