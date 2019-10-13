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
	WORD m_hardware_type;//hrd: hardware type.
	WORD m_protocol_type;//pro: protocol type
	BYTE m_hardware_address_length;//hln: length of each hardware address.
	BYTE m_protocol_address_length;//pln: length of each protocol address.
	WORD m_op_code;//op:opcode
	BYTE m_sender_hardware_address[6];//sha:hardware address of sender of this packet.
	DWORD m_sender_protocol_address;//protocol address of sender of this packet.
	BYTE m_target_hardware_address[6];//tha:hardware address of target of this packet.
	DWORD m_target_protocol_address;//protocol address of target.

public:
	ARP(void) { ; }
	~ARP(void) { ; }
	virtual int GetTotalLength() const { return 28; }//Total Length 28 bytes, 18 bytes padding needed for a Etherent frame
	virtual void SetTotalLength(int length) { ; }//nop
	virtual int GetHeaderLength() const { return 8; }//consider 8 bytes
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