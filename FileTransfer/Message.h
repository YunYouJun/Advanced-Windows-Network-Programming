#pragma once

#include <string.h>

#define COMMAND_DIR 1	// display file list
#define COMMAND_DOWNLOAD 2	// download file
#define COMMAND_EXIT 3	// exit client
#define COMMAND_TEST 4	// do test
#define FILE_DATA_SIZE 5	// msg content is file size, Data field is Null, and the Size field will store the size of file (instead data size)
#define FILE_DATA_REPLY 6	// the file data of server sending
#define FILE_DATA_ACK 7	// this msg is ack
#define FILE_DATA_END 8	// this is the end of file

#define MSG_HEADER_LENGTH 19	// 1 + 4 + 4 + 4 + 4 + 2
#define MAX_COMMAND_SIZE 256	// enough
#define MAX_BUF_SIZE 65535 - 20 - 8	// MAX IP Length is 65535, we need minus IP header 20 and UDP header 8
#define MAX_DATA_SIZE (MAX_BUF_SIZE - MSG_HEADER_LENGTH)	// then minus custom msg header 19 is our max data size

#define MAX_SEQ 15 // max seq and the max size of send window
#define NR_BUFS ((MAX_SEQ+1)/2) // half of seq, the max size of recv window

#define TIMEOUT_RESEND 40 // 40ms the time for resend

// header is 12 bytes
typedef struct MsgHeader {
	char Type;					// Message Type, 1b
	unsigned int seq;           // 4b
	unsigned int ack;           // 4b
	unsigned long Position;		// Identities the relative position in the downloading file  // 4b
	unsigned long Size;			// the size of the Data field of current Msg in a File Size Request/Rely Msg // 4b
                                // or file_data_size msg to reply file size
    unsigned short checksum;				// custom check data // 2b
}MessageHeader;

class Message
{
private:
	MessageHeader m_MessageHeader;
	char *m_Data;
public:
	Message();
	Message(char Type, unsigned long Position, unsigned long Size, char *Data, char Reserved = 0);
	~Message();

	void setType(char Type);
	void setSeq(unsigned int seq);
	void setAck(unsigned int ack);
	void setPosition(unsigned long Position);
	void setSize(unsigned long Size);
	void setData(char *Data);
    void setChecksum(unsigned short checksum);

	// ֻ��Ҫ���س�Ա�����ĳ�Ա�����ͱ�����Ϊ const ����
	char getType() const;
	unsigned int getSeq() const;
	unsigned int getAck() const;
	unsigned long getPosition() const;
	unsigned long getSize() const;
	char *getData() const;
    unsigned short getChecksum() const;

	void write(char *buffer);
	void read(char *buffer);
};