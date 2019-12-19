#pragma once
#include "URL.h"
#define TCP 1
#define UDP 0

class ClientSocket
{
public:
    char m_rtp_server_port[5];
    char m_rtp_server_ip[16];

    SOCKET RTSPSocket, RTPSocket;
    SOCKET ConnectSocket;
    struct addrinfo* RTSPServerInfo;
    struct addrinfo* RTPServerInfo;
    struct addrinfo* RTPClientInfo;
    struct sockaddr_in ClientAddr;
    socklen_t addrLen;
    static ClientSocket *instance;

    ClientSocket();
    ~ClientSocket();

    static ClientSocket* getInstace();
    bool Open(SOCKET &Socket, const char *ip, const char* port, int type);
    bool Bind(SOCKET &Socket, const char*port);
    bool Connect(SOCKET &Socket, struct addrinfo *serverInfo);
    bool Send(SOCKET &Socket, const char*sendbuf, int sendbuflen);
    string Recv(SOCKET &Socket);
    int RecvFrom(SOCKET &Socket, struct addrinfo *serverInfo, char *recvBuf, int revBuflen);
    void Close(SOCKET &Socket);
};

