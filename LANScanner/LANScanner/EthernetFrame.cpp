#include "stdafx.h"
#include "EthernetFrame.h"

EthernetFrame::EthernetFrame(void)
{
	memset(m_destination_address, 0, 6);
	memset(m_source_address, 0, 6);
	m_ether_type = 0;
	m_mac_data = NULL;
}

EthernetFrame::EthernetFrame(BYTE* DestAddress, BYTE* SourceAddress, WORD EtherType)
{
	memcpy(m_destination_address, DestAddress, 6);
	memcpy(m_source_address, SourceAddress, 6);
	m_ether_type = EtherType;
	m_mac_data = NULL;
	m_PADLength = 0;
}

void EthernetFrame::SetDestAddress(BYTE* DestAddress)
{
	memcpy(m_destination_address, DestAddress, 6);
}

void EthernetFrame::SetSourceAddress(BYTE* SourceAddress)
{
	memcpy(m_source_address, SourceAddress, 6);
}

void EthernetFrame::SetEtherType(WORD EtherType)
{
	m_ether_type = EtherType;
}

BOOL EthernetFrame::SetMACDATA(PDU* DATA)
{
	m_mac_data = DATA;
	return TRUE;
}

const PDU* EthernetFrame::GetMACDATA() const
{
	return m_mac_data;
}

BOOL EthernetFrame::Write(BYTE* link)
{
	memcpy(link, m_destination_address, 6);
	memcpy(link + 6, m_source_address, 6);
	memcpy(link + 12, &m_ether_type, 2);
	m_mac_data->Write(link + 14);
	return TRUE;
}

BOOL EthernetFrame::Read(BYTE* link)
{
	memcpy(m_destination_address, link, 6);
	memcpy(m_source_address, link + 6, 6);
	memcpy(&m_ether_type, link + 12, 2);
	m_mac_data->Read(link + 14);
	return TRUE;
}