#include "stdafx.h"
#include "IP.h"

BOOL IP::Write(BYTE *link)
{
	memcpy(link, &m_VersionIHL, 1);
	memcpy(link + 1, &m_DifferentiatedServices, 1);
	memcpy(link + 2, &m_Totallength, 2);
	memcpy(link + 4, &m_Identification, 2);
	memcpy(link + 6, &m_UnusedDFMFFragmentoffset, 2);
	memcpy(link + 8, &m_Timetolive, 1);
	memcpy(link + 9, &m_Protocol, 1);
	memcpy(link + 10, &m_Headerchecksum, 2);
	memcpy(link + 12, &m_Sourceaddress, 4);
	memcpy(link + 16, &m_Destinationaddress, 4);
	return TRUE;
}

BOOL IP::Read(BYTE *link)
{
	memcpy(&m_VersionIHL, link, 1);
	memcpy(&m_DifferentiatedServices, link + 1, 1);
	memcpy(&m_Totallength, link + 2, 2);
	memcpy(&m_Identification, link + 4, 2);
	memcpy(&m_UnusedDFMFFragmentoffset, link + 6, 2);
	memcpy(&m_Timetolive, link + 8, 1);
	memcpy(&m_Protocol, link + 9, 1);
	memcpy(&m_Headerchecksum, link + 10, 2);
	memcpy(&m_Sourceaddress, link + 12, 4);
	memcpy(&m_Destinationaddress, link + 16, 4);
	return TRUE;
}

void IP::setVersionIHL(BYTE VersionIHL)
{
	m_VersionIHL = VersionIHL;
}

void IP::setDifferentiatedServices(BYTE DifferentiatedServices)
{
	m_DifferentiatedServices = DifferentiatedServices;
}

void IP::setTotallength(WORD Totallength)
{
	m_Totallength = Totallength;
}

void IP::setIdentification(WORD Identification)
{
	m_Identification = Identification;
}

void IP::setUnusedDFMFFragmentoffset(WORD UnusedDFMFFragmentoffset)
{
	m_UnusedDFMFFragmentoffset = UnusedDFMFFragmentoffset;
}

void IP::setTimetolive(BYTE Timetolive)
{
	m_Timetolive = Timetolive;
}

void IP::setProtocol(BYTE Protocol)
{
	m_Protocol = Protocol;
}

void IP::setHeaderchecksum(WORD Headerchecksum)
{
	m_Headerchecksum = Headerchecksum;
}

void IP::setSourceaddress(DWORD Sourceaddress)
{
	m_Sourceaddress = Sourceaddress;
}

void IP::setDestinationaddress(DWORD Destinationaddress)
{
	m_Destinationaddress = Destinationaddress;
}

BYTE IP::getVersionIHL() const
{
	return m_VersionIHL;
}

BYTE IP::getDifferentiatedServices() const
{
	return m_DifferentiatedServices;
}

WORD IP::getTotallength() const
{
	return m_Totallength;
}

WORD IP::getIdentification()
{
	return m_Identification;
}

WORD IP::getUnusedDFMFFragmentoffset() const
{
	return m_UnusedDFMFFragmentoffset;
}

BYTE IP::getTimetolive() const
{
	return m_Timetolive;
}

BYTE IP::getProtocol() const
{
	return m_Protocol;
}

WORD IP::getHeaderchecksum() const
{
	return m_Headerchecksum;
}

DWORD IP::getSourceaddress() const
{
	return m_Sourceaddress;
}

DWORD IP::getDestinationaddress() const
{
	return m_Destinationaddress;
}