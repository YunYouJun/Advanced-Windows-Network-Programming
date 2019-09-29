#pragma once
// Unit 2 WinPCap.pdf
// http://tlc.cuc.edu.cn/Course/Course/showAction/id/30 ¿Î³Ì×ÊÁÏ
// P21-26

class PacketCapture
{
	pcap_if_t* alldevs;
	pcap_if_t* d;
	pcap_t* adhandle;
	int no = 0;
	char errbuf[PCAP_ERRBUF_SIZE];
public:
	~PacketCapture();
	int retrieveDeviceList();
	int selectAdapter();
	int getOrder();
	void getLocalMac(unsigned char* sha);
	unsigned long getLocalIp();
	int setFilter(const char* packet_filter);
	void sendPacket(const unsigned char* packet);
	unsigned long WINAPI receivePacket(LPVOID lpParam);
	float timeSub(const timeval& a, const timeval& b);
};