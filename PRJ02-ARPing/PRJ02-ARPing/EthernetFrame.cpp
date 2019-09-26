#include "EthernetFrame.h"

EthernetFrame::EthernetFrame() {
	memset(destination_address, 0, 6);
	memset(source_address, 0, 6);
	ether_type = ETHER_TYPE_ARP;
	data = new ARP();
}

EthernetFrame::EthernetFrame(unsigned char* da, unsigned char* sa, unsigned short et, ARP* arpData) {
	memcpy(destination_address, da, 6);
	memcpy(source_address, sa, 6);
	ether_type = ETHER_TYPE_ARP;
	data = arpData;
}

EthernetFrame::~EthernetFrame() {
	delete data;
}