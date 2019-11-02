#include "stdafx.h"
#include "TCP.h"

BOOL TCP::Write(BYTE *link)
{
	IP::Write(link);
	memcpy(link + 20, &m_Sourceport, 2);
	memcpy(link + 22, &m_Destinationport, 2);
	memcpy(link + 24, &m_Sequencenumber, 4);
	memcpy(link + 28, &m_Acknowledgementnumber, 4);
	memcpy(link + 32, &m_TCPheaderlengthUnused, 1);
	memcpy(link + 33, &m_UnusedFlags, 1);
	memcpy(link + 34, &m_Windowsize, 2);
	memcpy(link + 36, &m_Checksum, 2);
	memcpy(link + 38, &m_Urgentpointer, 2);
	return TRUE;
}

BOOL TCP::Read(BYTE *link)
{
	IP::Read(link);
	memcpy(&m_Sourceport, link + 20, 2);
	memcpy(&m_Destinationport, link + 22, 2);
	memcpy(&m_Sequencenumber, link + 24, 4);
	memcpy(&m_Acknowledgementnumber, link + 28, 4);
	memcpy(&m_TCPheaderlengthUnused, link + 32, 1);
	memcpy(&m_UnusedFlags, link + 33, 1);
	memcpy(&m_Windowsize, link + 34, 2);
	memcpy(&m_Checksum, link + 36, 2);
	memcpy(&m_Urgentpointer, link + 38, 2);
	return TRUE;
}

void TCP::setSourceport(WORD Sourceport)
{
	m_Sourceport = Sourceport;
}

void TCP::setDestinationport(WORD Destinationport)
{
	m_Destinationport = Destinationport;
}

void TCP::setSequencenumber(DWORD Sequencenumber)
{
	m_Sequencenumber = Sequencenumber;
}

void TCP::setAcknowledgementnumber(DWORD Acknowledgementnumber)
{
	m_Acknowledgementnumber = Acknowledgementnumber;
}

void TCP::setTCPheaderlengthUnused(BYTE TCPheaderlengthUnused)
{
	m_TCPheaderlengthUnused = TCPheaderlengthUnused;
}

void TCP::setUnusedFlags(BYTE UnusedFlags)
{
	m_UnusedFlags = UnusedFlags;
}

void TCP::setWindowsize(WORD Windowsize)
{
	m_Windowsize = Windowsize;
}

void TCP::setChecksum(WORD Checksum)
{
	m_Checksum = Checksum;
}

void TCP::setUrgentpointer(WORD Urgentpointer)
{
	m_Urgentpointer = Urgentpointer;
}

WORD TCP::getSourceport() const
{
	return m_Sourceport;
}

WORD TCP::getDestinationport() const
{
	return m_Destinationport;
}

DWORD TCP::getSequencenumber() const
{
	return m_Sequencenumber;
}

DWORD TCP::getAcknowledgementnumber() const
{
	return m_Acknowledgementnumber;
}

BYTE TCP::getTCPheaderlengthUnused() const
{
	return m_TCPheaderlengthUnused;
}

BYTE TCP::getUnusedFlags() const
{
	return m_UnusedFlags;
}

WORD TCP::getWindowsize() const
{
	return m_Windowsize;
}

WORD TCP::getChecksum() const
{
	return m_Checksum;
}

WORD TCP::getUrgentpointer() const
{
	return m_Urgentpointer;
}