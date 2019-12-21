#include "stdafx.h"
#include "ClientSocket.h"

ClientSocket::ClientSocket() {
    m_socket = INVALID_SOCKET;
}
ClientSocket::~ClientSocket() {}

bool ClientSocket::Open(int type)
{
	WSADATA wsaData;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		AfxMessageBox(L"Error at WSAStartup(): %d\n", iResult);
		return FALSE;
	}

    // Create a SOCKET for the server to listen for client connections
	if (type == TCP) {
		m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}
	if (type == UDP) {
		m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	}
    
    if (m_socket == INVALID_SOCKET) {
        AfxMessageBox(L"Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        return FALSE;
    }

    return TRUE;
}

// our program is client, and I don't want to use UDP, so we don't need it
bool ClientSocket::Bind(u_short port)
{
	// Bind the socket to any address and the specified port.
	sockaddr_in RecvAddr;
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(port);
	RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Setup the TCP listening socket
    int iResult;
    iResult = bind(m_socket, (sockaddr *)&RecvAddr, sizeof(RecvAddr));
    if (iResult == SOCKET_ERROR) {
		AfxMessageBox(L"bind failed with error: %u\n", WSAGetLastError());
        closesocket(m_socket);
        WSACleanup();
        return FALSE;
    }
    return TRUE;
}

bool ClientSocket::Connect(const char* ip, u_short port)
{
	int iResult;

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.S_un.S_addr = inet_addr(ip);

	// Connect to server.
	iResult = connect(m_socket, (sockaddr *)&server, sizeof(server));
	if (iResult == SOCKET_ERROR) {
		AfxMessageBox(L"Unable to connect to server!\n");
		return FALSE;
	}

	return TRUE;
}

bool ClientSocket::Send(const char*sendbuf, int sendbuflen)
{
    if (send(m_socket, sendbuf, sendbuflen, 0) == SOCKET_ERROR)
    {
        return FALSE;
    }
    return TRUE;
}

int ClientSocket::Recv(char* recvbuf)
{
    memset(recvbuf, 0, sizeof(recvbuf));
	return recv(m_socket, recvbuf, MAX_BUF_LEN, 0);
}

int ClientSocket::RecvFrom(char* recvbuf, const char* ip, u_short port) {
	sockaddr_in from;
	from.sin_family = AF_INET;
	from.sin_port = htons(port);
	from.sin_addr.S_un.S_addr = inet_addr(ip);

	int fromlen = sizeof(from);
	int recv_size = recvfrom(m_socket, recvbuf, MAX_BUF_LEN, 0, (SOCKADDR *)&from, &fromlen);
	return recv_size;
}

void ClientSocket::Close()
{
    closesocket(m_socket);
	m_socket = INVALID_SOCKET;
    WSACleanup();
}
