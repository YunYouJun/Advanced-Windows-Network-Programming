#pragma once
//interface for Protocol Data Unit
class PDU
{
public:
	virtual BOOL Write(BYTE *link) = 0;
	virtual BOOL Read(BYTE *link) = 0;
};