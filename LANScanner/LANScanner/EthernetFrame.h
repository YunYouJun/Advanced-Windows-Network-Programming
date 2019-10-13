#pragma once
#include "PDU.h"

static const WORD	ETHERTYPE_IPV4 = 0x0800;
static const WORD	ETHERTYPE_ARP = 0x0806;

class EthernetFrame
{
protected:
	BYTE m_destination_address[6];
	BYTE m_source_address[6];
	WORD m_ether_type;

	PDU* m_mac_data;
	int   m_PADLength;		//Length of PAD, range:0,46

public:
	EthernetFrame(void);
	EthernetFrame(BYTE* DestAddress, BYTE* SourceAddress, WORD EtherType = ETHERTYPE_IPV4);
	~EthernetFrame(void) { ; }

	// set and get
	void			SetDestAddress(BYTE* DestAddress);
	const BYTE*		GetDestAddress(void) const { ; }
	void			SetSourceAddress(BYTE* SourceAddress);
	const BYTE*		GetSourceAddress(void) const { ; }
	void			SetEtherType(WORD EtherType);
	WORD			GetEtherType() const { ; }

	BOOL			SetMACDATA(PDU* DATA);
	const PDU*		GetMACDATA() const;

	UINT			GetPADLength() const { return 18; }

	// input and output
	BOOL			Write(BYTE* link);
	BOOL			FindNextEtherFrame(BYTE* link) { ; }
	BOOL			Read(BYTE* link);

	void			PrintHeader() { ; }
	void			Print() { ; }

};