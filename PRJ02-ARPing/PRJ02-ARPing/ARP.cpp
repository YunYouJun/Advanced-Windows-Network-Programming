#include "ARP.h"

ARP::ARP() {
	hardware_type = 0x0;
	protocol_type = 0x0;
	hardware_address_length = 6;
	protocol_address_length = 4;
	operation_code = 0x0;
	memset(source_hardware_address, 0, 6);
	memset(source_protocol_address, 0, 4);
	memset(target_hardware_address, 0, 6);
	memset(target_protocol_address, 0, 4);
}

ARP::ARP(unsigned char* sha, unsigned char* spa, unsigned char* tha, unsigned char* tpa) {
	hardware_type = HARDWARE_TYPE_ETHERNET;
	protocol_type = PROTOCOL_TYPE_IP;
	hardware_address_length = HARDWARE_SIZE;
	protocol_address_length = PROTOCOL_SIZE;
	operation_code = OPCODE_REQUEST;
	memcpy(source_hardware_address, sha, 6);
	memcpy(source_protocol_address, spa, 4);
	memcpy(target_hardware_address, tha, 6);
	memcpy(target_protocol_address, tpa, 4);
}