#include "Helper.h"
#include "PacketCapture.h"
#include "EthernetFrame.h"

#include <iostream>
using namespace std;

PacketCapture::~PacketCapture() {
	pcap_freealldevs(alldevs); /* free the device list*/
}

int PacketCapture::retrieveDeviceList() {
	cout << "All adapters in your computer:" << endl;
	if (pcap_findalldevs_ex((char*)PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
	{
		fprintf(stderr, "Error in pcap_findalldevs_ex: %s\n", errbuf);
		return -1;
	}
	unsigned int i = 0;
	for (d = alldevs; d != NULL; d = d->next) { /* Print the list */
		printf("%d. %s", ++i, d->name);
		cout << d->addresses ->addr;

		if (d->description) {
			printf(" (%s)\n", d->description);
		}
		else {
			printf(" (No description available)\n");
		}
	}

	if (i == 0) {
		printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
		return -1;
	}
	return 0;
}

int PacketCapture::selectAdapter() {
	//int inum;
	//cout << "Please input number to choose your adapter: ";
	//cin >> inum;

	// change to auto
	//for (d = alldevs, no = 0; no < inum - 1; d = d->next, no++); /* Jump to the selected adapter */
	for (d = alldevs, no = 0; d; d = d->next, no++) {
		if (getLocalIp()) {
			break;
		}
		else {
			return -1;
		}
	}

	if (
		(adhandle = pcap_open(
			d->name, // name of the device
			65536, // portion of the packet to capture
			// 65536 guarantees that the whole packet
			PCAP_OPENFLAG_PROMISCUOUS, // promiscuous mode
			1000, // read timeout
			NULL, // authentication on the remote machine
			errbuf // error buffer
		)
			) == NULL)
	{
		fprintf(stderr, "\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
		return -1;
	}
	cout << "\nAuto choose adpter..." << endl;
	printf("listening on %s...\n", d->description);
	return 0;
}

int PacketCapture::getOrder() {
	return no;
}

void PacketCapture::getLocalMac(unsigned char* sha) {
	IP_ADAPTER_INFO adapter[5];
	unsigned long buflen = sizeof(adapter);
	unsigned long status = GetAdaptersInfo(adapter, &buflen);

	if (status == ERROR_SUCCESS) {
		PIP_ADAPTER_INFO pIpAdapterInfo = adapter;
		cout << "------------------------------------------------" << endl;
		unsigned int i = 0;
		string name(d->name);
		while (pIpAdapterInfo)
		{
			if (name.find(pIpAdapterInfo->AdapterName) != string::npos) {
				memcpy(sha, pIpAdapterInfo->Address, 6);
				break;
			}
			else {
				pIpAdapterInfo = pIpAdapterInfo->Next;
			}
			//cout << "Adapter No:" << "\t\t\t#" << ++i << endl;
			//cout << "Adapter Name:" << "\t\t\t" << pIpAdapterInfo->AdapterName << endl;
			//cout << "Adapter Description:" << "\t\t" << pIpAdapterInfo->Description << endl;
			//cout << "Adapter Hardware Address:" << "\t";
			//for (unsigned int i = 0; i < pIpAdapterInfo->AddressLength; i++) {
			//	if (i == pIpAdapterInfo->AddressLength - 1)
			//	{
			//		printf("%02x\n", pIpAdapterInfo->Address[i]);
			//	}
			//	else
			//	{
			//		printf("%02x-", pIpAdapterInfo->Address[i]);
			//	}
			//}
			//cout << "Adapter Ip Address£º" << "\t\t";
			//// network card maybe have multi-ip
			//IP_ADDR_STRING* pIpAddrString = &(pIpAdapterInfo->IpAddressList);
			//do
			//{
			//	cout << pIpAddrString->IpAddress.String << endl;
			//	pIpAddrString = pIpAddrString->Next;
			//} while (pIpAddrString);
			//pIpAdapterInfo = pIpAdapterInfo->Next;
			//cout << "------------------------------------------------" << endl;
		}

		//unsigned int no;
		//std::cin >> no;
		//pIpAdapterInfo = adapter;
		//for (unsigned int i = 1; i <= no; i++)
		//{
		//	if (i == no && pIpAdapterInfo) {
		//		memcpy(sha, pIpAdapterInfo->Address, 6);
		//	}
		//	else if(pIpAdapterInfo){
		//		pIpAdapterInfo = pIpAdapterInfo->Next;
		//	}
		//}
	}
}

unsigned long PacketCapture::getLocalIp() {
	pcap_addr_t* ipAddress;
	for (ipAddress = d->addresses; ipAddress; ipAddress = ipAddress->next) {
		// TCP/IP ¨C IPv4
		if (AF_INET == ipAddress->addr->sa_family) {
			if (ipAddress->addr) {
				return ((struct sockaddr_in*) ipAddress->addr)->sin_addr.S_un.S_addr;
			}
		}
	}
	return NULL;
}

// Flitering the traffic
int PacketCapture::setFilter(const char* packet_filter) {
	unsigned int netmask;
	struct bpf_program fcode;

	// Unit2 WinPCap.pdf P32
	if (d->addresses != NULL)
		/* Retrieve the mask of the first address of the interface */
		netmask = ((struct sockaddr_in*)(d->addresses->netmask))->sin_addr.S_un.S_addr;
	else
		/* If the interface is without an address we suppose to be in a C class network */
		netmask = 0xffffff;

	//compile the filter
	if (pcap_compile(adhandle, &fcode, packet_filter, 1, netmask) < 0)
	{
		fprintf(stderr, "\nUnable to compile the packet filter. Check the syntax.\n");
		return -1;
	}

	//set the filter
	if (pcap_setfilter(adhandle, &fcode) < 0)
	{
		fprintf(stderr, "\nError setting the filter.\n");
		return -1;
	}
	return 0;
}

void PacketCapture::sendPacket(const unsigned char* packet) {
	/* Send down the packet */
	if (pcap_sendpacket(adhandle, packet, 60 /* size */) != 0)
	{
		fprintf(stderr, "\nError sending the packet: %s\n", pcap_geterr(adhandle));
		return;
	}
}

unsigned long WINAPI PacketCapture::receivePacket(LPVOID lpParam)
{
	int res;
	struct pcap_pkthdr* header;
	const u_char* pkt_data;
	timeval send[4], receive[4];
	int i = 0, j = 0;
	unsigned char mac[6];
	ARP arp;

	/* Retrieve the packets */
	while ((res = pcap_next_ex(adhandle, &header, &pkt_data)) >= 0)
	{
		if (res == 0)
			/* Timeout elapsed */
			continue;

		EthernetFrame ref((unsigned char*)pkt_data);
		ref.dataToArp(arp);
		if (htons(OPCODE_REQUEST) == ref.getArpOperationCode())
		{
			send[i] = header->ts;
			++i;
		}
		if (htons(OPCODE_REPLY) == ref.getArpOperationCode())
		{
			receive[j] = header->ts;

			char IP[16];
			cout << "Unicast reply from " << Helper::unsignedLong2Char(ref.getArpSpa(), IP) << " ";
			memcpy(mac, ref.getArpSha(), 6);
			printf("[%02X:%02X:%02X:%02X:%02X:%02X]", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
			cout << " ";
			cout << timeSub(send[j], receive[j]) << "ms" << endl;
			++j;
		}

		if (i == 4 && j == 4)
		{
			cout << "Sent " << i << " probes (1 broadcast(s)) Received " << j <<" response(s)" << endl;
			break;
		}
	}

	if (res == -1) {
		printf("Error reading the packets: %s\n", pcap_geterr(adhandle));
		return -1;
	}

	return 0;
}

float PacketCapture::timeSub(const timeval& a, const timeval& b)
{
	return (1000 * (b.tv_sec - a.tv_sec) + (b.tv_usec - a.tv_usec) / (float)1000);
}