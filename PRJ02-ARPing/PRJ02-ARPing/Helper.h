#pragma once

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define HAVE_REMOTE
#include "pcap.h"
#pragma comment(lib, "wpcap.lib")

#include <Windows.h>
#include <iphlpapi.h>	// provide some function about network card & tcp/ip

class Helper {
public:
	static PCSTR unsignedLong2Char(const unsigned long ip, char* IP) {
		return inet_ntop(AF_INET, (void*)& ip, IP, 16);	// inet_ntoa() is deprecated API
	}
};