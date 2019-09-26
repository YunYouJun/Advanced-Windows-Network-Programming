#include "PacketCapture.h"

PacketCapture::~PacketCapture() {
	pcap_freealldevs(alldevs); /* free the device list*/
}

int PacketCapture::retrieveDeviceList() {
	if (pcap_findalldevs_ex((char *)PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
	{
		fprintf(stderr, "Error in pcap_findalldevs_ex: %s\n", errbuf);
		exit(1);
	}
	for (d = alldevs; d != NULL; d = d->next) { /* Print the list */
		printf("%d. %s", ++i, d->name);
		if (d->description)
			printf(" (%s)\n", d->description);
		else
			printf(" (No description available)\n");
	}
	if (i == 0) {
		printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
		return -1;
	}
	return 0;
}