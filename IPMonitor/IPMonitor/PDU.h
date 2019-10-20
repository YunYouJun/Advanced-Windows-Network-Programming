#pragma once

class PDU //interface for Protocol Data Unit
{
public:
	virtual BOOL Write(BYTE *link)= 0;
	virtual BOOL Read(BYTE *link)= 0;
};