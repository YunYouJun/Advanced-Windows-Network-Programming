/********************************************************************
	*	Advanced Windows Network Programming
	*	Project-02 ARPing
	*	201920081203009 杨睿 <me@yunyoujun.cn>

I declare that the assignment here submitted is original except for source material explicitly acknowledged.
I also acknowledge that I am aware of University policy and regulations on honesty in academic work,
and of the disciplinary guidelines and procedures applicable to breaches of such policy and regulations.
*********************************************************************/

#include "Helper.h"
#include "ARP.h"
#include "EthernetFrame.h"
#include "PacketCapture.h"

#include <iostream>
#include <string>
using namespace std;

#define FREQUENCY 4	// the number of times send packet

int main(int argc, char* argv[])
{
	string ip;
	if (argv[1]) {
		ip = argv[1];
	}
	else {
		cout << "Please input target IP: ";
		cin >> ip;
	}
	unsigned char tha[6];	// target hardware address
	memset(tha, 0xFF, 6);
	unsigned long tpa;
	inet_pton(AF_INET, ip.c_str(), (void*)& tpa);	// inet_addr() is deprecated API
	unsigned char sha[6];
	unsigned long spa;

	PacketCapture pc;
	pc.retrieveDeviceList();
	pc.selectAdapter();
	pc.getLocalMac(sha);
	spa = pc.getLocalIp();

	if (!spa) {
		cout << "IP is wrong. Please check your adapter.";
		return -1;
	}

	// packet
	ARP arp(sha, spa, tha, tpa);
	EthernetFrame ethernetFrame(tha, sha, ETHER_TYPE_ARP, &arp);

	// filter
	char sourceIP[16];
	Helper::unsignedLong2Char(spa, sourceIP);
	string targetIp(ip);
	string senderIp(sourceIP);
	string filter = "(arp) and (host " + senderIp + " or host " + targetIp + ")";
	if (pc.setFilter(filter.c_str())) {
		return -1;
	}

	// send packet
	unsigned char buffer[60] = { 0 };
	ethernetFrame.frameToByte(buffer);
	cout << "ARPING " << ip << " from " << sourceIP << " eth" << pc.getOrder() << endl;
	for (size_t i = 0; i < FREQUENCY; i++)
	{
		pc.sendPacket(buffer);
	}

	pc.receivePacket(NULL);
	return 0;
}