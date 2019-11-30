#include "pch.h"
#include "stdafx.h"
#include "CBlockingSocket.h"

CBlockingSocket::CBlockingSocket()
{
	m_ip = "127.0.0.1";
	m_port = 2121;
	m_socket = INVALID_SOCKET;
}

CBlockingSocket::CBlockingSocket(SOCKET socket)
{
	m_socket = socket;
}

CBlockingSocket::CBlockingSocket(const char *ip, u_short port)
{
	m_socket = INVALID_SOCKET;
	// init
	m_sin.sin_family = AF_INET;
	m_port = port;
	m_sin.sin_port = htons(m_port);
	if (ip) {
		// client
		m_sin.sin_addr.S_un.S_addr = inet_addr(ip);
	}
	else {
		// server
		// get hostname
		char hostname[256];
		gethostname(hostname, sizeof(hostname));
		// get local ip
		hostent *pHostent = gethostbyname(hostname);
		m_sin.sin_addr = *(in_addr*)pHostent->h_addr_list[0];
	}
	m_ip = inet_ntoa(m_sin.sin_addr);
}

int CBlockingSocket::Initialize()
{
	WSADATA wsaData;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	return 0;
}

BOOL CBlockingSocket::Open()
{
	// Create a SOCKET for the server to listen for client connections
	m_socket = socket(m_sin.sin_family, SOCK_STREAM, IPPROTO_TCP);

	if (m_socket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		return false;
	}

	return true;
}

BOOL CBlockingSocket::Bind()
{
	// server bind & client random
	int iResult;

	// Setup the TCP listening socket
	iResult = bind(m_socket, (sockaddr *)&m_sin, sizeof(m_sin));
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		return false;
	}

	return true;
}

BOOL CBlockingSocket::Listen()
{
	if (listen(m_socket, 5) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		return false;
	}

	return true;
}

CBlockingSocket CBlockingSocket::Accept(struct sockaddr &their_addr)
{
	int addr_size = sizeof(their_addr);
	SOCKET ClientSocket;
	ClientSocket = INVALID_SOCKET;

	// pdf 53
	fd_set fdread;
	int ret;
	FD_ZERO(&fdread);
	FD_SET(m_socket, &fdread);

	if ((ret = select(0, &fdread, NULL, NULL, NULL)) == SOCKET_ERROR)
	{
		return INVALID_SOCKET;
	}

	if (ret > 0)
	{
		if (FD_ISSET(m_socket, &fdread))
		{
			// Accept a client socket
			ClientSocket = accept(m_socket, &their_addr, &addr_size);
			if (ClientSocket == INVALID_SOCKET) {
				printf("Accept failed: %d\n", WSAGetLastError());
				return INVALID_SOCKET;
			}

			return CBlockingSocket(ClientSocket);
		}
	}

	return INVALID_SOCKET;
}

BOOL CBlockingSocket::Connect()
{
	int iResult;

	// Connect to server.
	iResult = connect(m_socket, (sockaddr *)&m_sin, sizeof(m_sin));
	if (iResult == SOCKET_ERROR) {
		printf("Unable to connect to server!\n");
		return false;
	}

	return true;
}

BOOL CBlockingSocket::Send(const char *sendbuf, long long sendbuflen)
{
	int iResult;
	// Send an initial buffer
	iResult = send(m_socket, sendbuf, sendbuflen, 0);
	if (iResult == SOCKET_ERROR) {
		printf("Send failed: %d\n", WSAGetLastError());
		return false;
	}
	return true;
}

int CBlockingSocket::Recv(char *recvbuf, int recvbuflen)
{
	int iResult;
	iResult = recv(m_socket, recvbuf, recvbuflen, 0);
	if (iResult > 0)
	{
		return iResult;
	}
	else if (iResult == 0)
	{
		printf("Connection closed.\n");
		return 0;
	}
	else
	{
		printf("Recv failed: %d\n", WSAGetLastError());
		return -1;
	}
}

BOOL CBlockingSocket::Close()
{
	// disable new sends, receives, or both, respectively.
	shutdown(m_socket, SD_BOTH);
	// to assure that all data is send and received on a connection, an application should call shutdown before calling closesocket
	if (closesocket(m_socket) == 0) {
		cout << "Connection is down." << endl;
		return true;
	}
	else {
		cout << "Close fail." << endl;
		return false;
	}
}

void CBlockingSocket::Cleanup()
{
	WSACleanup();
}

int CBlockingSocket::getSendRecvBuffer() const
{
	int nErrCode;
	int nBufLen;
	int nOptlen = sizeof(nBufLen);
	nErrCode = getsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, (char *)&nBufLen, &nOptlen);
	if (nErrCode == SOCKET_ERROR)
	{
		return 0;
	}
	return nBufLen;
}

int CBlockingSocket::setSendRecvBuffer(const int nBufLen)
{
	int nErrCode;
	int nOptlen = sizeof(nBufLen);
	nErrCode = setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, (char *)&nBufLen, nOptlen);
	if (nErrCode == SOCKET_ERROR)
	{
		return 1;
	}
	return 0;
}