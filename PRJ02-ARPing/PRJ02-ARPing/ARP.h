#pragma once

#include <iostream>

#define HARDWARE_TYPE_ETHERNET 0x0001
#define PROTOCOL_TYPE_IP 0x0800
#define HARDWARE_SIZE 6
#define PROTOCOL_SIZE 4
#define OPCODE_REQUEST 0x0001
#define OPCODE_REPLY 0x0002

class ARP
{
	// private by default
	unsigned short hardware_type;	// 2B
	unsigned short protocol_type;	// 2B
	unsigned char hardware_address_length;	// 1B
	unsigned char protocol_address_length;	// 1B
	unsigned short operation_code;	// 2B
	unsigned char source_hardware_address[6];	// 6B mac
	unsigned char source_protocol_address[4];	// 4B ip
	unsigned char target_hardware_address[6];	// 6B mac
	unsigned char target_protocol_address[4];	// 4B ip

public:
	ARP();
	ARP(unsigned char* sha, unsigned char* spa, unsigned char* tha, unsigned char* tpa);
};

#pragma pack()