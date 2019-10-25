#pragma once

#include "PDU.h"

class IP : public PDU
{
protected:
	// Internet Header Length
	BYTE m_VersionIHL;//4(version)+4(IHL)
	BYTE m_DifferentiatedServices;
	WORD m_Totallength;
	WORD m_Identification;
	WORD m_UnusedDFMFFragmentoffset;//1+1(DF)+1(MF)+13(fragment offset)
	BYTE m_Timetolive;
	BYTE m_Protocol;
	WORD m_Headerchecksum;
	DWORD m_Sourceaddress;
	DWORD m_Destinationaddress;
public:
	IP(void) { ; }
	~IP(void) { ; }
	virtual BOOL Write(BYTE *link);
	virtual BOOL Read(BYTE *link);

	void setVersionIHL(BYTE versionIHL);
	void setDifferentiatedServices(BYTE DifferentiatedServices);
	void setTotallength(WORD Totallength);
	void setIdentification(WORD Identification);
	void setUnusedDFMFFragmentoffset(WORD UnusedDFMFFragmentoffset);
	void setTimetolive(BYTE Timetolive);
	void setProtocol(BYTE Protocol);
	void setHeaderchecksum(WORD Headerchecksum);
	void setSourceaddress(DWORD Sourceaddress);
	void setDestinationaddress(DWORD Destinationaddress);

	BYTE getVersionIHL() const;
	BYTE getDifferentiatedServices() const;
	WORD getTotallength() const;
	WORD getIdentification() const;
	WORD getUnusedDFMFFragmentoffset() const;
	BYTE getTimetolive() const;
	BYTE getProtocol() const;
	WORD getHeaderchecksum() const;
	DWORD getSourceaddress() const;
	DWORD getDestinationaddress() const;
};