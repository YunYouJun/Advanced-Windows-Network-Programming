#pragma once
#include "PDU.h"

//https://blog.csdn.net/eydwyz/article/details/65446328
static const WORD ETHERTYPE_IPV4 = 0x0800;
static const WORD ETHERTYPE_IPV6 = 0x86DD;
//static const WORD ETHERTYPE_ARP = 0x0806;

class EthernetFrame
{
protected:
	BYTE m_DestAddress[6];
	BYTE m_SourceAddress[6];
	WORD m_EtherType;
	PDU * m_MACDATA;

public:
	EthernetFrame(void);
	EthernetFrame(BYTE *DestAddress, BYTE *SourceAddress, WORD EtherType = ETHERTYPE_IPV4);
	~EthernetFrame(void) { ; }

	// set and get
	void			SetDestAddress(BYTE *DestAddress);
	const BYTE *	GetDestAddress(void) const { ; }
	void			SetSourceAddress(BYTE *SourceAddress);
	const BYTE *	GetSourceAddress(void) const { ; }
	void			SetEtherType(WORD EtherType);
	WORD			GetEtherType() const { ; }

	BOOL			SetMACDATA(PDU * DATA);
	const PDU *		GetMACDATA() const;

	// input and output
	BOOL			Write(BYTE *link);// from preamble to Frame Check Sum
	BOOL			FindNextEtherFrame(BYTE *link) { ; }
	BOOL			Read(BYTE *link);
};