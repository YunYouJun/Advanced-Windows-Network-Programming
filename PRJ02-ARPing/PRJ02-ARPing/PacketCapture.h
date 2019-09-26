#pragma once

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define HAVE_REMOTE
#include "pcap.h"
#pragma comment(lib, "wpcap.lib")

// Unit 2 WinPCap.pdf
// http://tlc.cuc.edu.cn/Course/Course/showAction/id/30 ¿Î³Ì×ÊÁÏ
// P21-26

class PacketCapture
{
	pcap_if_t* alldevs;
	pcap_if_t* d;
	int i = 0;
	char errbuf[PCAP_ERRBUF_SIZE];

public:
	~PacketCapture();
	int retrieveDeviceList();
};

