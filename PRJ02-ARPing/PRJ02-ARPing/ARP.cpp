#include "ARP.h"

ARP::ARP() {
	hardware_type = 0x0;
	protocol_type = 0x0;
	hardware_address_length = HARDWARE_SIZE;
	protocol_address_length = PROTOCOL_SIZE;
	operation_code = 0x0;
	memset(sender_hardware_address, 0, 6);
	sender_protocol_address = 0;
	memset(target_hardware_address, 0, 6);
	target_protocol_address = 0;
}

ARP::ARP(unsigned char* sha, unsigned long spa, unsigned char* tha, unsigned long tpa) {
	hardware_type = htons(HARDWARE_TYPE_ETHERNET);
	protocol_type = htons(PROTOCOL_TYPE_IP);
	hardware_address_length = HARDWARE_SIZE;
	protocol_address_length = PROTOCOL_SIZE;
	operation_code = htons(OPCODE_REQUEST);
	memcpy(sender_hardware_address, sha, 6);
	sender_protocol_address = spa;
	memcpy(target_hardware_address, tha, 6);
	target_protocol_address = tpa;
}

ARP::ARP(unsigned short ht, unsigned short pt, unsigned char hal, unsigned char pal, unsigned short oc, unsigned char* sha, unsigned long spa, unsigned char* tha, unsigned long tpa) {
	hardware_type = ht;
	protocol_type = pt;
	hardware_address_length = hal;
	protocol_address_length = pal;
	operation_code = oc;
	memcpy(sender_hardware_address, sha, 6);
	sender_protocol_address = spa;
	memcpy(target_hardware_address, tha, 6);
	target_protocol_address = tpa;
}

unsigned short ARP::getOperationCode() {
	return operation_code;
}

unsigned char* ARP::getSha() {
	return sender_hardware_address;
}

unsigned long ARP::getSpa() {
	return sender_protocol_address;
}