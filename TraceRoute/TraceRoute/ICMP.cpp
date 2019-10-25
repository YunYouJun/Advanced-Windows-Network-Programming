#include "stdafx.h"
#include "ICMP.h"

BOOL ICMP::Write(BYTE *link)
{
	IP::Write(link);
	memcpy(link + 20, &m_type, 1);
	memcpy(link + 21, &m_code, 1);
	memcpy(link + 22, &m_checksum, 2);
	memcpy(link + 24, &m_id, 2);
	memcpy(link + 26, &m_seq, 2);
	memcpy(link + 28, m_data, 64);
	return TRUE;
}

BOOL ICMP::Read(BYTE *link)
{
	IP::Read(link);
	memcpy(&m_type, link + 20, 1);
	memcpy(&m_code, link + 21, 1);
	memcpy(&m_checksum, link + 22, 2);
	memcpy(&m_id, link + 24, 2);
	memcpy(&m_seq, link + 26, 2);
	memcpy(m_data, link + 28, 64);
	return TRUE;
}

void ICMP::setType(BYTE type)
{
	m_type = type;
}

void ICMP::setCode(BYTE code)
{
	m_code = code;
}

void ICMP::setChecksum(WORD checksum)
{
	m_checksum = checksum;
}

void ICMP::setId(WORD id)
{
	m_id = id;
}

void ICMP::setSeq(WORD seq)
{
	m_seq = seq;
}

void ICMP::setData(BYTE data[])
{
	memcpy(m_data, data, 64);
}

BYTE ICMP::getType() const
{
	return m_type;
}

BYTE ICMP::getCode() const
{
	return m_code;
}

WORD ICMP::getChecksum() const
{
	return m_checksum;
}

WORD ICMP::getId() const
{
	return m_id;
}

WORD ICMP::getSeq() const
{
	return m_seq;
}

const BYTE *ICMP::getData() const
{
	return m_data;
}