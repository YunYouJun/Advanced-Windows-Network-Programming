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

// our program is client, and I don't want to use UDP, so we don't need it
bool ClientSocket::Bind(const char *ip, u_short port)
{
    // server bind & client random
    sockaddr_in service;
    service.sin_family = AF_INET;

    service.sin_addr.s_addr = inet_addr(ip);
    service.sin_port = htons(port);
    // connect

    // Setup the TCP listening socket
    int iResult;
    iResult = bind(m_tcp_socket, (sockaddr *)&service, sizeof(service));
    if (iResult == SOCKET_ERROR) {
		AfxMessageBox(L"bind failed with error: %u\n", WSAGetLastError());
        closesocket(m_tcp_socket);
        WSACleanup();
        return FALSE;
    }
    return TRUE;
}

bool ClientSocket::Connect(const char* ip, u_short port)
{
	int iResult;

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.S_un.S_addr = inet_addr(ip);

	// Connect to server.
	iResult = connect(m_tcp_socket, (sockaddr *)&server, sizeof(server));
	if (iResult == SOCKET_ERROR) {
		AfxMessageBox(L"Unable to connect to server!\n");
		return FALSE;
	}

	return TRUE;
}

bool ClientSocket::Send(const char*sendbuf, int sendbuflen)
{
    if (send(m_tcp_socket, sendbuf, sendbuflen, 0) == SOCKET_ERROR)
    {
        return FALSE;
    }
    return TRUE;
}

int ClientSocket::Recv(char* recvbuf)
{
    memset(recvbuf, 0, sizeof(recvbuf));
	return recv(m_tcp_socket, recvbuf, MAX_BUF_LEN, 0);
}

int ClientSocket::RecvRTP(char* recvbuf)
{
    memset(recvbuf, 0, sizeof(recvbuf));
    int recv_size = recv(m_tcp_socket, recvbuf, sizeof(recvbuf), 0);
    //if (recv_size == SOCKET_ERROR) {
    //    return "Recv RTP Error";
    //}
    return recv_size;
}

void ClientSocket::Close()
{
    closesocket(m_tcp_socket);
	m_tcp_socket = INVALID_SOCKET;
    WSACleanup();
}
