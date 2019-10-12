#include "pch.h"
#include "Device.h"

int Device::obtainDeviceList()
{
	int i = 0;
	char errbuf[PCAP_ERRBUF_SIZE];
	/* Retrieve the device list from the local machine */
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL /* auth is not needed */, &alldevs, errbuf) == -1)
		return -1;

	/* Print the list */
	for (d = alldevs; d != NULL; d = d->next)
		++i;

	if (i == 0)
		return -1;

	return 0;
}

int Device::openAdapter(const char* nam)
{
	char errbuf[PCAP_ERRBUF_SIZE];

	/* Jump to the selected adapter */
	for (d = alldevs; d != NULL; d = d->next)
	{
		char* name = strchr(d->name, '{');
		if (0 == strcmp(name, nam))
			break;
	}

	/* Open the device */
	if (d) {
		if ((adhandle = pcap_open(
			d->name,          // name of the device
			65536,            // portion of the packet to capture
							  // 65536 guarantees that the whole packet will be captured on all the link layers
			PCAP_OPENFLAG_PROMISCUOUS,    // promiscuous mode
			1000,             // read timeout
			NULL,             // authentication on the remote machine
			errbuf            // error buffer
		)) == NULL)
			return -1;
	}

	return 0;
}

int Device::setFilter(const char* packet_filter)
{
	u_int netmask;
	struct bpf_program fcode;

	if (d->addresses != NULL)
		/* Retrieve the mask of the first address of the interface */
		netmask = ((struct sockaddr_in*)(d->addresses->netmask))->sin_addr.S_un.S_addr;
	else
		/* If the interface is without an address we suppose to be in a C class network */
		netmask = 0xffffff;


	//compile the filter
	if (pcap_compile(adhandle, &fcode, packet_filter, 1, netmask) < 0)
	{
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	//set the filter
	if (pcap_setfilter(adhandle, &fcode) < 0)
	{
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}
	return 0;
}

void Device::sendPacket(const BYTE* packet)
{
	/* Send down the packet */
	if (pcap_sendpacket(adhandle, packet, 60 /* size */) != 0)
		return;
}

pcap_t* Device::getHandle() const
{
	return adhandle;
}