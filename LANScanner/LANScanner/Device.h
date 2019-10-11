#include "Common.h"
#include "ARP.h"

class Device
{
private:
	pcap_if_t* alldevs;
	pcap_if_t* d;
	pcap_t* adhandle;
public:
	int obtainDeviceList();
	int openAdapter(const char* nam);
	int setFilter(const char* packet_filter);
	void sendPacket(const BYTE* packet);
	pcap_t* getHandle() const;
};