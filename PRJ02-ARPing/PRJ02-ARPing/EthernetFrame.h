#pragma once

#include "ARP.h"

#define ETHER_TYPE_ARP 0x0806

class EthernetFrame
{
	unsigned char destination_address[6];
	unsigned char source_address[6];
	unsigned short ether_type;
	ARP* data;
public:
	EthernetFrame();
	EthernetFrame(unsigned char* da, unsigned char* sa, unsigned short et, ARP* arpData);
	~EthernetFrame();
};

