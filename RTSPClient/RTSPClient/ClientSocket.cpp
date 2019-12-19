#include "stdafx.h"
#include "ClientSocket.h"

ClientSocket *ClientSocket::instance = NULL;

ClientSocket::ClientSocket() {}
ClientSocket::~ClientSocket() {}

ClientSocket* ClientSocket::getInstace() {
    if (!instance) {
        instance = new ClientSocket();
    }
    return instance;
}

bool ClientSocket::Open(SOCKET &Socket, const char *ip, const char*port, int type)
{
    int iResult;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));

    WSADATA wsaData;
    WORD wversionRequested = MAKEWORD(2, 2);
    if (WSAStartup(wversionRequested, &wsaData) != 0)
    {
        return FALSE;
    }

    if (type == TCP)
    {
        if ((Socket = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
        {
            WSACleanup();
            return FALSE;
        }
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        iResult = getaddrinfo(ip, port, &hints, &RTSPServerInfo);
        if (iResult != 0)
        {
            return FALSE;
        }
    }
    if (type == UDP)
    {
        if ((Socket = socket(AF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR)
        {
            WSACleanup();
            return FALSE;
        }
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_protocol = IPPROTO_UDP;
        iResult = getaddrinfo(ip, port, &hints, &RTPServerInfo);

        memcpy(m_rtp_server_ip, ip, 16);
        memcpy(m_rtp_server_port, port, sizeof(port));

        if (iResult != 0)
        {
            return FALSE;
        }
    }
    return TRUE;
}

bool ClientSocket::Bind(SOCKET &Socket, const char*port)
{
    char name[128] = { 0 };
    gethostname(name, sizeof(name));
    int iResult;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    iResult = getaddrinfo(name, port, &hints, &RTPClientInfo);
    if (iResult != 0)
    {
        return FALSE;
    }
    if (bind(Socket, RTPClientInfo->ai_addr, RTPClientInfo->ai_addrlen) != 0)
    {
        closesocket(Socket);
        WSACleanup();
        return FALSE;
    }
    return TRUE;
}

bool ClientSocket::Connect(SOCKET &Socket, struct addrinfo *serverInfo)
{
    if (connect(Socket, serverInfo->ai_addr, serverInfo->ai_addrlen) != 0)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

bool ClientSocket::Send(SOCKET &Socket, const char*sendbuf, int sendbuflen)
{
    int size;
    size = send(Socket, sendbuf, sendbuflen, 0);
    if (size == SOCKET_ERROR)
    {
        return FALSE;
    }
    return TRUE;
}

string ClientSocket::Recv(SOCKET &Socket)
{
    int size;
    char recvbuf[65535];
    memset(recvbuf, 0, sizeof(recvbuf));
    size = recv(Socket, recvbuf, sizeof(recvbuf), 0);
    if (size == SOCKET_ERROR)
    {
        return "Socket Error.";
    }
    else if (size == 0)
    {
        return "CLOSE";
    }
    else
    {
        if (recvbuf[0] == '$' && recvbuf[1] == '\x1') {
            // interleaved rtsp
            char temp[65475];
            memset(temp, 0, sizeof(temp));
            for (size_t i = 0; i < size - 60; i++)
            {
                temp[i] = recvbuf[i + 60];
            }
            return temp;
        }
        return recvbuf;
    }
}

int ClientSocket::RecvFrom(SOCKET &Socket, struct addrinfo *serverInfo, char *recvBuf, int revBuflen)
{
    int size = sizeof(serverInfo->ai_addr);
    sockaddr_in server_info;
    server_info.sin_addr.S_un.S_addr = inet_addr(m_rtp_server_ip);
    server_info.sin_port = atoi(m_rtp_server_port);

    int ClientBytesRecv = recvfrom(Socket, recvBuf, revBuflen, 0, (SOCKADDR *)&server_info, &size);

    if (ClientBytesRecv == SOCKET_ERROR) {
        return -1;
    }
    else {
        return ClientBytesRecv;
    }
}

void ClientSocket::Close(SOCKET &Socket)
{
    closesocket(Socket);
    Socket = 0;
    WSACleanup();
}
