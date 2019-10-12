#include "pch.h"
#include "ARP.h"

//(BYTE *)ARP强制转换，需要对ARP类进行#pragma pack(1)，但是这里用到了虚函数，ARP对象最前会有4字节的虚指针，容易出错。这样写的话就不需要#pragma pack(1)了
BOOL ARP::Write(BYTE* link)
{
	memcpy(link, &m_hardware_type, 2);
	memcpy(link + 2, &m_protocol_type, 2);
	memcpy(link + 4, &m_hardware_address_length, 1);
	memcpy(link + 5, &m_protocol_address_length, 1);
	memcpy(link + 6, &m_op_code, 2);
	memcpy(link + 8, m_sender_hardware_address, 6);
	memcpy(link + 14, &m_sender_protocol_address, 4);
	memcpy(link + 18, m_target_hardware_address, 6);
	memcpy(link + 24, &m_target_protocol_address, 4);
	return TRUE;
}

BOOL ARP::Read(BYTE* link)
{
	memcpy(&m_hardware_type, link, 2);
	memcpy(&m_protocol_type, link + 2, 2);
	memcpy(&m_hardware_address_length, link + 4, 1);
	memcpy(&m_protocol_address_length, link + 5, 1);
	memcpy(&m_op_code, link + 6, 2);
	memcpy(m_sender_hardware_address, link + 8, 6);
	memcpy(&m_sender_protocol_address, link + 14, 4);
	memcpy(m_target_hardware_address, link + 18, 6);
	memcpy(&m_target_protocol_address, link + 24, 4);
	return TRUE;
}

void ARP::setHardwareType(WORD HardwareType)
{
	m_hardware_type = HardwareType;
}

void ARP::setProtocolType(WORD ProtocolType)
{
	m_protocol_type = ProtocolType;
}

void ARP::setHardwareAddrLen(BYTE HardwareAddrLen)
{
	m_hardware_address_length = HardwareAddrLen;
}

void ARP::setProtocolAddrLen(BYTE ProtocolAddrLen)
{
	m_protocol_address_length = ProtocolAddrLen;
}

void ARP::setOpCode(WORD OpCode)
{
	m_op_code = OpCode;
}

void ARP::setSenderHardwareAddr(BYTE* SenderHardwareAddr)
{
	memcpy(m_sender_hardware_address, SenderHardwareAddr, 6);
}

void ARP::setSenderProtocolAddr(DWORD SenderProtocolAddr)
{
	m_sender_protocol_address = SenderProtocolAddr;
}

void ARP::setTargetHardwareAddr(BYTE* TargetHardwareAddr)
{
	memcpy(m_target_hardware_address, TargetHardwareAddr, 6);
}

void ARP::setTargetProtocolAddr(DWORD TargetProtocolAddr)
{
	m_target_protocol_address = TargetProtocolAddr;
}

WORD ARP::getHardwareType() const
{
	return m_hardware_type;
}

WORD ARP::getProtocolType() const
{
	return m_protocol_type;
}

BYTE ARP::getHardwareAddrLen() const
{
	return m_hardware_address_length;
}

BYTE ARP::getProtocolAddrLen() const
{
	return m_protocol_address_length;
}

WORD ARP::getOpCode() const
{
	return m_op_code;
}

const BYTE* ARP::getSenderHardwareAddr() const
{
	return m_sender_hardware_address;
}

DWORD ARP::getSenderProtocolAddr() const
{
	return m_sender_protocol_address;
}

const BYTE* ARP::getTargetHardwareAddr() const
{
	return m_target_hardware_address;
}

DWORD ARP::getTargetProtocolAddr() const
{
	return m_target_protocol_address;
}