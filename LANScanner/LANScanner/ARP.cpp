#include "pch.h"
#include "ARP.h"

//(BYTE *)ARP强制转换，需要对ARP类进行#pragma pack(1)，但是这里用到了虚函数，ARP对象最前会有4字节的虚指针，容易出错。这样写的话就不需要#pragma pack(1)了
BOOL ARP::Write(BYTE* link)
{
	memcpy(link, &m_HardwareType, 2);
	memcpy(link + 2, &m_ProtocolType, 2);
	memcpy(link + 4, &m_HardwareAddrLen, 1);
	memcpy(link + 5, &m_ProtocolAddrLen, 1);
	memcpy(link + 6, &m_OpCode, 2);
	memcpy(link + 8, m_SenderHardwareAddr, 6);
	memcpy(link + 14, &m_SenderProtocolAddr, 4);
	memcpy(link + 18, m_TargetHardwareAddr, 6);
	memcpy(link + 24, &m_TargetProtocolAddr, 4);
	return TRUE;
}

BOOL ARP::Read(BYTE* link)
{
	memcpy(&m_HardwareType, link, 2);
	memcpy(&m_ProtocolType, link + 2, 2);
	memcpy(&m_HardwareAddrLen, link + 4, 1);
	memcpy(&m_ProtocolAddrLen, link + 5, 1);
	memcpy(&m_OpCode, link + 6, 2);
	memcpy(m_SenderHardwareAddr, link + 8, 6);
	memcpy(&m_SenderProtocolAddr, link + 14, 4);
	memcpy(m_TargetHardwareAddr, link + 18, 6);
	memcpy(&m_TargetProtocolAddr, link + 24, 4);
	return TRUE;
}

void ARP::setHardwareType(WORD HardwareType)
{
	m_HardwareType = HardwareType;
}

void ARP::setProtocolType(WORD ProtocolType)
{
	m_ProtocolType = ProtocolType;
}

void ARP::setHardwareAddrLen(BYTE HardwareAddrLen)
{
	m_HardwareAddrLen = HardwareAddrLen;
}

void ARP::setProtocolAddrLen(BYTE ProtocolAddrLen)
{
	m_ProtocolAddrLen = ProtocolAddrLen;
}

void ARP::setOpCode(WORD OpCode)
{
	m_OpCode = OpCode;
}

void ARP::setSenderHardwareAddr(BYTE* SenderHardwareAddr)
{
	memcpy(m_SenderHardwareAddr, SenderHardwareAddr, 6);
}

void ARP::setSenderProtocolAddr(DWORD SenderProtocolAddr)
{
	m_SenderProtocolAddr = SenderProtocolAddr;
}

void ARP::setTargetHardwareAddr(BYTE* TargetHardwareAddr)
{
	memcpy(m_TargetHardwareAddr, TargetHardwareAddr, 6);
}

void ARP::setTargetProtocolAddr(DWORD TargetProtocolAddr)
{
	m_TargetProtocolAddr = TargetProtocolAddr;
}

WORD ARP::getHardwareType() const
{
	return m_HardwareType;
}

WORD ARP::getProtocolType() const
{
	return m_ProtocolType;
}

BYTE ARP::getHardwareAddrLen() const
{
	return m_HardwareAddrLen;
}

BYTE ARP::getProtocolAddrLen() const
{
	return m_ProtocolAddrLen;
}

WORD ARP::getOpCode() const
{
	return m_OpCode;
}

const BYTE* ARP::getSenderHardwareAddr() const
{
	return m_SenderHardwareAddr;
}

DWORD ARP::getSenderProtocolAddr() const
{
	return m_SenderProtocolAddr;
}

const BYTE* ARP::getTargetHardwareAddr() const
{
	return m_TargetHardwareAddr;
}

DWORD ARP::getTargetProtocolAddr() const
{
	return m_TargetProtocolAddr;
}