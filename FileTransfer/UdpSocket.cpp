#include "stdafx.h"
#include "pch.h"
#include "UdpSocket.h"

BOOL UdpSocket::Open() {
		//int socket(int domain, int type, int protocol);
		m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (m_socket < 0) {
			perror("socket error");
			return false;
		}
		return true;
	}

BOOL UdpSocket::Bind(const char *ip, u_short port) {
		m_addr.sin_family = AF_INET;
		m_addr.sin_port = htons(port);
		if (ip) {
			m_addr.sin_addr.S_un.S_addr = inet_addr(ip);
		}
		else {
			char hostname[256];
			gethostname(hostname, sizeof(hostname));
			hostent *pHostent = gethostbyname(hostname);
			m_addr.sin_addr = *(in_addr*)pHostent->h_addr_list[0];
		}

		m_port = port;
		m_ip = inet_ntoa(m_addr.sin_addr);
		
		//int bind(int sockfd, struct sockaddr *addr, socklen_t addrlen);
		int ret = bind(m_socket, (struct sockaddr*)&m_addr, sizeof(m_addr));
		if (ret < 0) {
			perror("bind error");
			return false;
		}
		return true;
	}

void UdpSocket::Select() {
    // pdf 53
    fd_set fdread;
    int ret;
    timeval tv;
    FD_ZERO(&fdread);
    FD_SET(m_socket, &fdread);

    tv.tv_sec = 1;
    tv.tv_usec = 0;
    if ((ret = select(0, &fdread, NULL, NULL, &tv)) == SOCKET_ERROR)
    {
        cout << "We need resend!" << endl;
    }
}

BOOL UdpSocket::Close() {
	closesocket(m_socket);
	m_socket = -1;
	return true;
}