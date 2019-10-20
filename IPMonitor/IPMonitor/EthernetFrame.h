#pragma once
#include "PDU.h"

//https://blog.csdn.net/eydwyz/article/details/65446328
static const WORD	ETHERTYPE_IPV4 = 0x0800;
static const WORD	ETHERTYPE_IPV6 = 0x86DD;
static const WORD	ETHERTYPE_ARP = 0x0806;

//class EthernetFrame
//{
//protected:
//	BYTE m_destination_address[6];
//	BYTE m_source_address[6];
//	WORD m_ether_type;
//
//	PDU* m_mac_data;
//	//Length of PAD, range:0,46
//	int   m_PADLength;
//
//public:
//	EthernetFrame(void);
//	EthernetFrame(BYTE* DestAddress, BYTE* SourceAddress, WORD EtherType = ETHERTYPE_IPV4);
//	~EthernetFrame(void) { ; }
//
//	// set and get
//	void			SetDestAddress(BYTE* DestAddress);
//	const BYTE*		GetDestAddress(void) const { ; }
//	void			SetSourceAddress(BYTE* SourceAddress);
//	const BYTE*		GetSourceAddress(void) const { ; }
//	void			SetEtherType(WORD EtherType);
//	WORD			GetEtherType() const { ; }
//
//	BOOL			SetMACDATA(PDU* DATA);
//	const PDU*		GetMACDATA() const;
//
//	UINT			GetPADLength() const { return 18; }
//
//	// input and output
//	BOOL			Write(BYTE* link);
//	BOOL			FindNextEtherFrame(BYTE* link) { ; }
//	BOOL			Read(BYTE* link);
//
//	void			PrintHeader() { ; }
//	void			Print() { ; }
//
//};

class EthernetFrame
{
protected:
	//BYTE Preamble[7];	//The Preamble field is a 7-octet field,10101010,0xAA 

	//BYTE SFD;		//StartFrameDelimiter:The SFD field is the sequence 10101011 0xAB, It immediately follows the preamble pattern. A MAC frame starts immediately after the SFD.

	BYTE m_DestAddress[6];	//The Destination Address field shall specify the destination addressee(s) for which the MAC frame is intended.
	BYTE m_SourceAddress[6];	//The Source Address field shall identify the station from which the MAC frame was initiated.
							//Each address field shall be 48 bits in length.
							//Each octet of each address field shall be transmitted least significant bit first.

	WORD m_EtherType;			//BYTE EtherType[2];	

	PDU * m_MACDATA;			//MAC Client Data,  46 to 1500 or 1504 or 1982 octects (including PAD)

	int	  m_MACDATALength;	//Length of MACDATA(no padding), range 46, 1500 (including PAD), we just only deal with basic frame
	int   m_PADLength;		//Length of PAD, range:0,46

	//DWORD FCS;				//Frame Check Sum Sequence

public:
	EthernetFrame(void);
	EthernetFrame(BYTE *DestAddress, BYTE *SourceAddress, WORD EtherType = ETHERTYPE_IPV4);
	~EthernetFrame(void) { ; }

	//Access methods
	void			SetDestAddress(BYTE *DestAddress);
	const BYTE *	GetDestAddress(void) const { ; }
	void			SetSourceAddress(BYTE *SourceAddress);
	const BYTE *	GetSourceAddress(void) const { ; }
	void			SetEtherType(WORD EtherType);
	WORD			GetEtherType() const { ; }

	BOOL			SetMACDATA(PDU * DATA);
	const PDU *		GetMACDATA() const;

	UINT			GetMACDATALength() const { return 28; }
	UINT			GetPADLength() const { return 18; } //We can not get the PAD Length if we read a fake frame.

	//DWORD			GetFCS() const;

	//Input/Output methods
	BOOL			Write(BYTE *link);//Write out Whole Frame: from preamble to FCS
	BOOL			FindNextEtherFrame(BYTE *link) { ; }//Find the position of next Etherframe in the incoming buffer;
	BOOL			Read(BYTE *link);	//read whole frame

	void			PrintHeader() { ; }
	void			Print() { ; }

};