#pragma once

#include "PDU.h"

class IP : public PDU
{
protected:
	BYTE m_VersionIHL;//4+4 maybe should devide?
	BYTE m_DifferentiatedServices;
	WORD m_Totallength;
	WORD m_Identification;
	WORD m_UnusedDFMFFragmentoffset;//1+1(DF)+1(MF)+13(Fragment offset)
	BYTE m_Timetolive;
	BYTE m_Protocol;
	WORD m_Headerchecksum;
	DWORD m_Sourceaddress;
	DWORD m_Destinationaddress;
	// options
public:
	IP(void){;}
	~IP(void){;}
	virtual BOOL Write(BYTE *link);
	virtual BOOL Read(BYTE *link);

	void setProtocol(BYTE Protocol);
	void setSourceaddress(DWORD Sourceaddress);
	void setDestinationaddress(DWORD Destinationaddress);

	BYTE getProtocol() const;
	DWORD getSourceaddress() const;
	DWORD getDestinationaddress() const;
};