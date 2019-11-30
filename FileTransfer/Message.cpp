#include "pch.h"
#include "Message.h"

Message::Message()
{
	m_MessageHeader.Type = 0;
    m_MessageHeader.seq = 0;
    m_MessageHeader.ack = 0;
	m_MessageHeader.Position = 0;
	m_MessageHeader.Size = 0;
    m_MessageHeader.checksum = 0;
	m_Data = NULL;
}

Message::Message(char Type, unsigned long Position, unsigned long Size, char *Data, char Reserved)
{
	m_MessageHeader.Type = Type;
	m_MessageHeader.Position = Position;
	m_MessageHeader.Size = Size;
	m_Data = Data;
}

Message::~Message()
{

}

void Message::setType(char Type)
{
	m_MessageHeader.Type = Type;
}

void Message::setSeq(unsigned int seq)
{
	m_MessageHeader.seq = seq;
}

void Message::setAck(unsigned int ack)
{
	m_MessageHeader.ack = ack;
}

void Message::setPosition(unsigned long Position)
{
	m_MessageHeader.Position = Position;
}

void Message::setSize(unsigned long Size)
{
	m_MessageHeader.Size = Size;
}

void Message::setChecksum(unsigned short checksum)
{
    m_MessageHeader.checksum = checksum;
}

void Message::setData(char *Data)
{
	m_Data = Data;
}

char Message::getType() const
{
	return m_MessageHeader.Type;
}

unsigned int Message::getSeq() const
{
	return m_MessageHeader.seq;
}

unsigned int Message::getAck() const
{
	return m_MessageHeader.ack;
}

unsigned long Message::getPosition() const
{
	return m_MessageHeader.Position;
}

unsigned long Message::getSize() const
{
	return m_MessageHeader.Size;
}

unsigned short Message::getChecksum() const
{
    return m_MessageHeader.checksum;
}

char *Message::getData() const
{
	return m_Data;
}

void Message::write(char *buffer)
{
	memcpy(buffer, &m_MessageHeader.Type, 1);
	memcpy(buffer + 1, &m_MessageHeader.seq, 4);
    memcpy(buffer + 5, &m_MessageHeader.ack, 4);
	memcpy(buffer + 9, &m_MessageHeader.Position, 4);
	memcpy(buffer + 13, &m_MessageHeader.Size, 4);
    memcpy(buffer + 17, &m_MessageHeader.checksum, 2);
	if (m_Data)
		memcpy(buffer + MSG_HEADER_LENGTH, m_Data, m_MessageHeader.Size);
}

void Message::read(char *buffer)
{
	memcpy(&m_MessageHeader.Type, buffer, 1);
	memcpy(&m_MessageHeader.seq, buffer + 1, 4);
	memcpy(&m_MessageHeader.ack, buffer + 5, 4);
	memcpy(&m_MessageHeader.Position, buffer + 9, 4);
	memcpy(&m_MessageHeader.Size, buffer + 13, 4);
	memcpy(&m_MessageHeader.checksum, buffer + 17, 2);
	m_Data = buffer + MSG_HEADER_LENGTH;
}