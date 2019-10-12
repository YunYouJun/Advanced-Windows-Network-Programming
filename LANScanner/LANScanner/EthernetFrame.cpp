#include "pch.h"
#include "EthernetFrame.h"

EthernetFrame::EthernetFrame(void)
{
	memset(m_DestAddress, 0, 6);
	memset(m_SourceAddress, 0, 6);
	m_EtherType = 0;
	m_MACDATA = NULL;
	m_MACDATALength = 0;
	//m_PADLength = 0;
}

EthernetFrame::EthernetFrame(BYTE* DestAddress, BYTE* SourceAddress, WORD EtherType)
{
	memcpy(m_DestAddress, DestAddress, 6);
	memcpy(m_SourceAddress, SourceAddress, 6);
	m_EtherType = EtherType;
	m_MACDATA = NULL;
	m_MACDATALength = 0;
	m_PADLength = 0;
}

void EthernetFrame::SetDestAddress(BYTE* DestAddress)
{
	memcpy(m_DestAddress, DestAddress, 6);
}

void EthernetFrame::SetSourceAddress(BYTE* SourceAddress)
{
	memcpy(m_SourceAddress, SourceAddress, 6);
}

void EthernetFrame::SetEtherType(WORD EtherType)
{
	m_EtherType = EtherType;
}

BOOL EthernetFrame::SetMACDATA(PDU* DATA)
{
	m_MACDATA = DATA;
	return TRUE;
}

const PDU* EthernetFrame::GetMACDATA() const
{
	return m_MACDATA;
}

BOOL EthernetFrame::Write(BYTE* link)
{
	memcpy(link, m_DestAddress, 6);
	memcpy(link + 6, m_SourceAddress, 6);
	memcpy(link + 12, &m_EtherType, 2);
	m_MACDATA->Write(link + 14);
	return TRUE;
}

BOOL EthernetFrame::Read(BYTE* link)
{
	memcpy(m_DestAddress, link, 6);
	memcpy(m_SourceAddress, link + 6, 6);
	memcpy(&m_EtherType, link + 12, 2);
	m_MACDATA->Read(link + 14);
	return TRUE;
}