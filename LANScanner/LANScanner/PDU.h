#pragma once

// protocol data unit
class PDU
{
public:
	virtual int GetTotalLength() const = 0;
	virtual void SetTotalLength(int length) = 0;
	virtual int GetHeaderLength() const = 0;
	virtual void SetHeaderLength(int length) = 0;
	virtual void PrintHeader() const = 0;
	virtual void Print() const = 0;
	virtual int Write(unsigned char* link) = 0;
	virtual int Read(unsigned char* link) = 0;
};
