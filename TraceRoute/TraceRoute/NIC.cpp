#include "stdafx.h"
#include "NIC.h"

NIC::~NIC()
{
	if (pAdapterInfo)
		free(pAdapterInfo);
}

int NIC::initNIC()
{
	/* Declare and initialize variables */
	pAdapter = NULL;
	DWORD dwRetVal = 0;

	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	pAdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL)
		return 1;

	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
		if (pAdapterInfo == NULL)
			return 1;
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
		pAdapter = pAdapterInfo;

	return 0;
}

vector<char *> NIC::getAdapterList()
{
	vector<char *> temp;
	while (pAdapter) {
		temp.push_back(pAdapter->Description);
		pAdapter = pAdapter->Next;
	}
	return temp;
}

void NIC::selectNIC(int index)
{
	int count = 0;
	pAdapter = pAdapterInfo;//backup
	while (pAdapter) {
		if (count == index)
			break;
		pAdapter = pAdapter->Next;
		++count;
	}
}

char *NIC::getNICname() const
{
	return pAdapter->AdapterName;
}

char *NIC::getNICdescription() const
{
	if (NULL == pAdapter)
		return NULL;
	return pAdapter->Description;
}

char *NIC::getIPAddress() const
{
	if (NULL == pAdapter)
		return NULL;
	return pAdapter->IpAddressList.IpAddress.String;
}

char *NIC::getSubnetMask() const
{
	if (NULL == pAdapter)
		return NULL;
	return pAdapter->IpAddressList.IpMask.String;
}

BYTE *NIC::getMacAddress() const
{
	if (NULL == pAdapter)
		return NULL;
	static BYTE mac[6] = { 0 };
	pAdapter->AddressLength;
	memcpy(mac, pAdapter->Address, pAdapter->AddressLength);
	return mac;
}

char *NIC::getGatewayIPAddress() const
{
	if (NULL == pAdapter)
		return NULL;
	return pAdapter->GatewayList.IpAddress.String;
}

BYTE *NIC::getGatewayMac() const
{
	DWORD dwRetVal;
	IPAddr DestIp = 0;
	IPAddr SrcIp = 0;       /* default for src ip */
	ULONG MacAddr[2];       /* for 6-byte hardware addresses */
	ULONG MacAddrLen = 6;  /* default to length of six bytes */

	BYTE *bMacAddr;

	//将一个点分十进制的IP转换成一个长整数型数
	inet_pton(AF_INET, getGatewayIPAddress(), &DestIp);
	inet_pton(AF_INET, getIPAddress(), &SrcIp);

	memset(&MacAddr, 0xff, sizeof(MacAddr));

	dwRetVal = SendARP(DestIp, SrcIp, &MacAddr, &MacAddrLen);

	if (dwRetVal == NO_ERROR)
	{
		bMacAddr = (BYTE *)& MacAddr;
		if (MacAddrLen)
		{
			static BYTE mac[6] = { 0 };
			memcpy(mac, bMacAddr, MacAddrLen);
			return mac;
		}
	}

	return NULL;
}

BYTE *NIC::getLanMac(DWORD someip) const
{
	DWORD dwRetVal;
	IPAddr DestIp = 0;
	IPAddr SrcIp = 0;       /* default for src ip */
	ULONG MacAddr[2];       /* for 6-byte hardware addresses */
	ULONG MacAddrLen = 6;  /* default to length of six bytes */

	BYTE *bMacAddr;

	DestIp = someip;
	inet_pton(AF_INET, getIPAddress(), &SrcIp);
	memset(&MacAddr, 0xff, sizeof(MacAddr));

	dwRetVal = SendARP(DestIp, SrcIp, &MacAddr, &MacAddrLen);

	if (dwRetVal == NO_ERROR)
	{
		bMacAddr = (BYTE *)& MacAddr;
		if (MacAddrLen)
		{
			static BYTE mac[6] = { 0 };
			memcpy(mac, bMacAddr, MacAddrLen);
			return mac;
		}
	}

	return NULL;
}