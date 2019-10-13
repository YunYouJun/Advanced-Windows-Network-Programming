#pragma once

#include "PDU.h"

#pragma pack(1)

static const WORD HRD_ETHERNET = 1; // Ethernet
static const WORD PRO_IP = 0x0800;

// Opcodes.
static const WORD OP_REQUEST = 1;//ARP REQUEST
static const WORD OP_REPLY = 2;//ARP REPLY

class ARP : public PDU
{
protected:
	WORD m_hardware_type;
	WORD m_protocol_type;
	BYTE m_hardware_address_length;
	BYTE m_protocol_address_length;
	WORD m_op_code;
	BYTE m_sender_hardware_address[6];
	DWORD m_sender_protocol_address;
	BYTE m_target_hardware_address[6];
	DWORD m_target_protocol_address;

public:
	ARP(void) { ; }
	~ARP(void) { ; }
	virtual int GetTotalLength() const { return 28; }
	virtual void SetTotalLength(int length) { ; }
	virtual int GetHeaderLength() const { return 8; }
	virtual void SetHeaderLength(int length) { ; }
	virtual void PrintHeader() const { ; }
	virtual void Print() const { ; }
	virtual BOOL Write(BYTE* link);
	virtual BOOL Read(BYTE* link);

	void setHardwareType(WORD HardwareType);
	void setProtocolType(WORD ProtocolType);
	void setHardwareAddrLen(BYTE HardwareAddrLen);
	void setProtocolAddrLen(BYTE ProtocolAddrLen);
	void setOpCode(WORD OpCode);
	void setSenderHardwareAddr(BYTE* SenderHardwareAddr);
	void setSenderProtocolAddr(DWORD SenderProtocolAddr);
	void setTargetHardwareAddr(BYTE* TargetHardwareAddr);
	void setTargetProtocolAddr(DWORD TargetProtocolAddr);

	WORD getHardwareType() const;
	WORD getProtocolType() const;
	BYTE getHardwareAddrLen() const;
	BYTE getProtocolAddrLen() const;
	WORD getOpCode() const;
	const BYTE* getSenderHardwareAddr() const;
	DWORD getSenderProtocolAddr() const;
	const BYTE* getTargetHardwareAddr() const;
	DWORD getTargetProtocolAddr() const;
};