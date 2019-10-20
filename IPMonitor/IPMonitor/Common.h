#pragma once

#ifndef HAVE_REMOTE
#define HAVE_REMOTE
#endif

#include <stdio.h>
#include <string>
#include <vector>

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "pcap.h"
#pragma comment(lib, "wpcap.lib")

#include <Windows.h>
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")

using namespace std;

class Common
{
public:
	static char* mac2str(const BYTE* mac)
	{
		if (NULL == mac)
			return "00-00-00-00-00-00";
		static char temp[18] = { 0 };
		sprintf_s(temp, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		return temp;
	}

	//static PCSTR dword2char(const DWORD ip, char* IP)
	//{
	//	return inet_ntop(AF_INET, (void*)&ip, IP, 16);	// inet_ntoa() is deprecated API
	//}

	static char *dword2char(const DWORD ip)
	{
		struct in_addr addr;
		addr.S_un.S_addr = ip;
		return inet_ntoa(addr);
	}

	// https://www.iana.org/assignments/protocol-numbers/protocol-numbers.xhtml
	static char* GetProtocol(BYTE Protocol) {
		switch (Protocol)
		{
		case 1: return "ICMP";
		case 2: return "IGMP";
		case 4: return "IP in IP";
		case 6: return "TCP";
		case 8: return "EGP";
		case 17: return "UDP";
		case 41: return "IPv6";
		default: return "UNKNOW";
		}
	}
};