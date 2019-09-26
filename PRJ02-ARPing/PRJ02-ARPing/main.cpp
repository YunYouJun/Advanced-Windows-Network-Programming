#include "ARP.h"
#include "EthernetFrame.h"
#include "PacketCapture.h"
using namespace std;

int main(int argc, char *argv[])
{
	string ip;
	if (argv[1]) {
		ip = argv[1];
	}
	else {
		ip = "baidu.com";
	}
	cout << ip << endl;

	unsigned char tha[6];	// target hardware address
	memset(tha, 0xFF, 6);


	EthernetFrame testFrame;
	//ARP arp(sha, spa, tha, tpa);
	PacketCapture pc;
	pc.retrieveDeviceList();

	return 0;
}