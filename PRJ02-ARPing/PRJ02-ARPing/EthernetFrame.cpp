#include "EthernetFrame.h"
#include <winsock2.h>

EthernetFrame::EthernetFrame() {
	memset(destination_address, 0, 6);
	memset(source_address, 0, 6);
	ether_type = 0x0;
	data = new ARP();
}

EthernetFrame::EthernetFrame(unsigned char* da, unsigned char* sa, unsigned short et, ARP* arpData) {
	memcpy(destination_address, da, 6);
	memcpy(source_address, sa, 6);
	ether_type = htons(ETHER_TYPE_ARP);
	data = arpData;
}

EthernetFrame::EthernetFrame(unsigned char* buffer)
{
	memcpy(destination_address, buffer, 6);
	memcpy(source_address, buffer + 6, 6);
	memcpy(&ether_type, buffer + 12, 2);
	data = new ARP();
	memcpy(data, buffer + 14, 28);
}

void EthernetFrame::frameToByte(unsigned char* buffer) {
	memcpy(buffer, destination_address, 6);
	memcpy(buffer + 6, source_address, 6);
	memcpy(buffer + 12, &ether_type, 2);
	memcpy(buffer + 14, data, 28);
	memset(buffer + 42, 0, 18);
}

unsigned short EthernetFrame::getArpOperationCode() {
	return data->getOperationCode();
}

void EthernetFrame::dataToArp(ARP& arp) {
	unsigned short hardware_type = 0x0;
	unsigned short protocol_type = 0x0;
	unsigned char hardware_address_length = 6;
	unsigned char protocol_address_length = 4;
	unsigned short operation_code = 0x0;
	unsigned char sender_hardware_address[6] = { 0 };
	unsigned long sender_protocol_address = 0;
	unsigned char target_hardware_address[6] = { 0 };
	unsigned long target_protocol_address = 0;

	memcpy(&hardware_type, data, 2);
	memcpy(&protocol_type, data + 2, 2);
	memcpy(&hardware_address_length, data + 4, 1);
	memcpy(&protocol_address_length, data + 5, 1);
	memcpy(&operation_code, data + 6, 2);
	memcpy(sender_hardware_address, data + 8, 6);
	memcpy(&sender_protocol_address, data + 14, 4);
	memcpy(target_hardware_address, data + 18, 6);
	memcpy(&target_protocol_address, data + 24, 4);
	arp = ARP(hardware_type, protocol_type, hardware_address_length, protocol_address_length, operation_code, sender_hardware_address, sender_protocol_address, target_hardware_address, target_protocol_address);
}

unsigned char* EthernetFrame::getArpSha() {
	return data->getSha();
}

unsigned long EthernetFrame::getArpSpa() {
	return data->getSpa();
}