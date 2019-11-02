#pragma once
#include "IP.h"

class TCP : public IP
{
private:
	WORD m_Sourceport;
	WORD m_Destinationport;
	DWORD m_Sequencenumber;
	DWORD m_Acknowledgementnumber;
	BYTE m_TCPheaderlengthUnused;//4+4
	BYTE m_UnusedFlags;//2+6
	WORD m_Windowsize;
	WORD m_Checksum;
	WORD m_Urgentpointer;
public:
	TCP(void) { ; }
	~TCP(void) { ; }
	virtual int GetTotalLength() const { return 0; }
	virtual void SetTotalLength(int length) { ; }
	virtual int GetHeaderLength() const { return 0; }
	virtual void SetHeaderLength(int length) { ; }
	virtual void PrintHeader() const { ; }
	virtual void Print() const { ; }
	virtual BOOL Write(BYTE *link);
	virtual BOOL Read(BYTE *link);

	void setSourceport(WORD Sourceport);
	void setDestinationport(WORD Destinationport);
	void setSequencenumber(DWORD Sequencenumber);
	void setAcknowledgementnumber(DWORD Acknowledgementnumber);
	void setTCPheaderlengthUnused(BYTE TCPheaderlengthUnused);
	void setUnusedFlags(BYTE UnusedFlags);
	void setWindowsize(WORD Windowsize);
	void setChecksum(WORD Checksum);
	void setUrgentpointer(WORD Urgentpointer);

	WORD getSourceport() const;
	WORD getDestinationport() const;
	DWORD getSequencenumber() const;
	DWORD getAcknowledgementnumber() const;
	BYTE getTCPheaderlengthUnused() const;
	BYTE getUnusedFlags() const;
	WORD getWindowsize() const;
	WORD getChecksum() const;
	WORD getUrgentpointer() const;
};