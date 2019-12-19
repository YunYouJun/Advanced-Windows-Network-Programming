#include "stdafx.h"
#include "ClientSocket.h"

ClientSocket::ClientSocket() {
    m_tcp_socket = INVALID_SOCKET;
}
ClientSocket::~ClientSocket() {}

bool ClientSocket::Initialize()
{
    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        AfxMessageBox(L"Error at WSAStartup(): %d\n", iResult);
        return FALSE;
    }
    return TRUE;
}

bool ClientSocket::Open()
{
    // Create a SOCKET for the server to listen for client connections
    m_tcp_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (m_tcp_socket == INVALID_SOCKET) {
        AfxMessageBox(L"Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        return FALSE;
    }

    return TRUE;
}

bool ClientSocket::Bind(const char *ip, u_short port)
{
    // server bind & client random
    sockaddr_in service;
    service.sin_family = AF_INET;

    // to fix
    //service.sin_addr.s_addr = inet_addr(ip);
    //service.sin_port = htons(port);
    // connect

    // Setup the TCP listening socket
    int iResult;
    iResult = bind(m_tcp_socket, (sockaddr *)&service, sizeof(service));
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %u\n", WSAGetLastError());
        closesocket(m_tcp_socket);
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

char* ClientSocket::Recv(SOCKET &Socket)
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
            for (int i = 0; i < size - 60; i++)
            {
                temp[i] = recvbuf[i + 60];
            }
            return temp;
        }
        return recvbuf;
    }
}

int ClientSocket::RecvRTP(SOCKET &Socket, char* recvbuf)
{
    memset(recvbuf, 0, sizeof(recvbuf));
    int recv_size = recv(Socket, recvbuf, sizeof(recvbuf), 0);
    //if (recv_size == SOCKET_ERROR) {
    //    return "Recv RTP Error";
    //}
    return recv_size;
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
