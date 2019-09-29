#pragma once

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define HAVE_REMOTE
#include "pcap.h"
#pragma comment(lib, "wpcap.lib")

#include <Windows.h>
#include <iphlpapi.h>	// 提供了一些实用的获取网络信息（网卡、tcp/ip等）的函数

class Helper {
public:
	static PCSTR unsignedLong2Char(const unsigned long ip, char* IP) {
		return inet_ntop(AF_INET, (void*)& ip, IP, 16);	// inet_ntoa() is deprecated API
	}
};