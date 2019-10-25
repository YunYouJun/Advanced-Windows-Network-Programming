#pragma once

#include "Common.h"

class NIC
{
private:
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter;
public:
	~NIC();
	int initNIC();
	vector<char *> getAdapterList();
	void selectNIC(int index);
	char *getNICname() const;
	char *getNICdescription() const;
	char *getIPAddress() const;
	char *getSubnetMask() const;
	BYTE *getMacAddress() const;
	char *getGatewayIPAddress() const;
	BYTE *getGatewayMac() const;
	BYTE *getLanMac(DWORD someip) const;
};