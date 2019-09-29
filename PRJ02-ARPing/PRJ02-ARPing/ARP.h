#pragma once

#include <winsock2.h>

#define HARDWARE_TYPE_ETHERNET 0x0001
#define PROTOCOL_TYPE_IP 0x0800
#define HARDWARE_SIZE 0x6
#define PROTOCOL_SIZE 0x4
#define OPCODE_REQUEST 0x0001
#define OPCODE_REPLY 0x0002

#pragma pack(1)
class ARP
{
	// private by default
	unsigned short hardware_type;	// 2B
	unsigned short protocol_type;	// 2B
	unsigned char hardware_address_length;	// 1B
	unsigned char protocol_address_length;	// 1B
	unsigned short operation_code;	// 2B
	unsigned char sender_hardware_address[6];	// 6B mac
	unsigned long sender_protocol_address;	// 4B ip
	unsigned char target_hardware_address[6];	// 6B mac
	unsigned long target_protocol_address;	// 4B ip

public:
	ARP();
	ARP(unsigned char* sha, unsigned long spa, unsigned char* tha, unsigned long tpa);
	ARP(unsigned short ht, unsigned short pt, unsigned char hal, unsigned char pal, unsigned short oc, unsigned char* sha, unsigned long spa, unsigned char* tha, unsigned long tpa);
	unsigned short getOperationCode();
	unsigned char* getSha();
	unsigned long getSpa();
};

#pragma pack()