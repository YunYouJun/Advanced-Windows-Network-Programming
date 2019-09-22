/********************************************************************
	*    Advanced Windows Network Programming
  *    Project-01 Frame Parser
	*    201920081203009 杨睿 <me@yunyoujun.cn>

I declare that the assignment here submitted is original except for source material explicitly acknowledged. 
I also acknowledge that I am aware of University policy and regulations on honesty in academic work, 
and of the disciplinary guidelines and procedures applicable to breaches of such policy and regulations. 
*********************************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

#define PREAMBLE_SIZE 7
#define ADDRESS_SIZE 6
#define TYPE_SIZE 2
#define FCS_SIZE 4

long long fileEnd;
unsigned int Crc32Table[256];

struct frame {
	unsigned char no;	// Frame#
	unsigned char preamble[7];
	unsigned char start;
	unsigned char da[6];	// destination address
	unsigned char sa[6];	// source address
	unsigned char type[2];	// type
	vector<unsigned char> data;	// data
	unsigned char fcs[4];	// frame check sum
	unsigned int rr;	// Recalculate Result
	frame();
};

frame::frame() {
	no = 0;
	preamble[0] = 0xAA;
	for (int i = 0; i < PREAMBLE_SIZE; i++) {
		preamble[i] = 0xAA;
	}
	start = 0xAB;
	da[0] = 0x0;
	sa[0] = 0x0;
	type[0] = 0x0;
	fcs[0] = 0x0;
	rr = 0x0;
}

bool CRCCheck(frame &ethernetFrame);
bool IsFrameStart(ifstream &fin);
frame ParseFrame(ifstream &fin, unsigned int no);
void InitCrcTable();
void PrintFrame(frame ethernetFrame);
void ReadFrame(string frameFilename);

int main(int argc, char* argv[]) {
	string frameFilename;
	cout << "Frame Parser:" << endl;
	cout << "-------------------------------------------------" << endl;
	if (argv[1]) {
		frameFilename = argv[1];
		cout << "Parse frame filename: " << frameFilename << endl;
	}
	else {
		cout << "Please input frame filename: ";
		cin >> frameFilename;
	}
	cout << "-------------------------------------------------\n" << endl;
	//frameFilename = "testFrame";
	ReadFrame(frameFilename);
	return 0;
}

void ReadFrame(string frameFilename) {
	ifstream fin;
	fin.open(frameFilename, ios::binary);

	if (!fin.is_open())
	{
		cout << "Failed to open file, please check if file is exist or not." << endl;
		return;
	}

	fin.seekg(0, ios::end);
	fileEnd = fin.tellg();	// get the end position of file
	fin.seekg(0, ios::beg);

	InitCrcTable();	// init CRC32 table
	unsigned int no = 0;
	while (IsFrameStart(fin)) {
		no++;
		PrintFrame(ParseFrame(fin, no));
	}
	
	cout << endl;
	cout << "Analysis Finished!" << endl;
	fin.close();
}

frame ParseFrame(ifstream &fin,unsigned int no) {
	frame ethernetFrame;
	ethernetFrame.no = no;
	for (int i = 0; i < ADDRESS_SIZE; i++) {
		ethernetFrame.da[i] = fin.get();
	}
	for (int i = 0; i < ADDRESS_SIZE; i++) {
		ethernetFrame.sa[i] = fin.get();
	}
	for (int i = 0; i < TYPE_SIZE; i++) {
		ethernetFrame.type[i] = fin.get();
	}

	long long dataStart = fin.tellg();
	long long dataEnd;

	if (IsFrameStart(fin)) {
		dataEnd = fin.tellg();
		fin.seekg(dataStart, ios::beg);
		for (int i = 0; i < dataEnd - dataStart - FCS_SIZE - PREAMBLE_SIZE - 1; i++) {
			ethernetFrame.data.push_back(fin.get());
		}
	}
	else {
		fin.seekg(dataStart, ios::beg);
		for (int i = 0; i < fileEnd - dataStart - FCS_SIZE; i++) {
			ethernetFrame.data.push_back(fin.get());
		}
	}
	for (int i = FCS_SIZE - 1; i >= 0; i--) {
		ethernetFrame.fcs[i] = fin.get();
	}
	return ethernetFrame;
}

void PrintFrame(frame ethernetFrame) {
	unsigned int i;
	cout << "Frame#:" << "\t\t\t" << (int)ethernetFrame.no << endl;
	cout << "Preamble:" << "\t\t";
	for (i = 0; i < PREAMBLE_SIZE; i++) {
		cout << hex << uppercase << (int)ethernetFrame.preamble[i] << " ";
	}
	cout << endl;
	cout << "Start:" << "\t\t\t" << hex << uppercase << (int)ethernetFrame.start << endl;
	cout << "Destination Address:" << "\t";
	for (int i = 0; i < ADDRESS_SIZE; i++) {
		cout << setw(2) << setfill('0') << hex << uppercase << (int)ethernetFrame.da[i];
		if (i != ADDRESS_SIZE - 1) {
			cout << "-";
		}
		else {
			cout << endl;
		}
	}
	cout << "Source Address:" << "\t\t";
	for (i = 0; i < ADDRESS_SIZE; i++) {
		cout << setw(2) << setfill('0') << hex << uppercase << (int)ethernetFrame.sa[i];
		if (i != ADDRESS_SIZE - 1) {
			cout << "-";
		}
		else {
			cout << endl;
		}
	}
	cout << "Type:" << "\t\t\t";
	for (i = 0; i < TYPE_SIZE; i++) {
		cout << setw(2) << setfill('0') << hex << uppercase << (int)ethernetFrame.type[i];
		if (i != TYPE_SIZE - 1) {
			cout << " ";
		}
		else {
			cout << endl;
		}
	}
	cout << "Data:" << "\t\t\t";
	for (i = 0; i < ethernetFrame.data.size(); i++) {
		cout << ethernetFrame.data[i];
	}
	cout << endl;
	cout << "FCS:" << "\t\t\t" << "0x ";
	for (i = 0; i < FCS_SIZE; i++) {
		cout << hex << nouppercase << (int)ethernetFrame.fcs[i];
	}
	cout << endl;
	if (CRCCheck(ethernetFrame)) {
		cout << "CRC32 Check Correct!" << endl;
	}
	else
	{
		cout << "CRC32 Check Wrong!" << endl;
		cout << "Recalculate Result:" << "\t" << "0x " << ethernetFrame.rr << endl;
	}
	cout << "\n-------------------------------------------------\n" << endl;
}

bool IsFrameStart(ifstream &fin) {
	int p;
	// avoid redundance
	while (true) {
		for (int i = 0; i < PREAMBLE_SIZE; i++) {
			// cannot use eof
			if (fin.tellg() >= fileEnd) {
				return false;
			}
			p = fin.get();
			if (p != 0xAA) {
				i = -1;	// keep seaching
			}
		}
		if (fin.tellg() >= fileEnd) {
			return false;
		}
		if (fin.get() == 0xAB) {
			return true;
		}
	}
	return false;
}

// ref from https://blog.csdn.net/u013898698/article/details/78864161
// init crc table
void InitCrcTable()
{
	unsigned int c;
	unsigned int i, j;

	for (i = 0; i < 256; i++) {
		c = (unsigned int)i;
		for (j = 0; j < 8; j++) {
			if (c & 1)
				c = 0xedb88320L ^ (c >> 1);
			else
				c = c >> 1;
		}
		Crc32Table[i] = c;
	}
}

unsigned int GetCrc32(char* InStr, unsigned int dataLen) {
	unsigned int i;
	unsigned int Crc;
	// calculate crc32 check sum
	Crc = 0xffffffff;
	for (i = 0; i < dataLen; i++) {
		Crc = Crc32Table[(Crc ^ InStr[i]) & 0xff] ^ (Crc >> 8);
	}
	Crc ^= 0xffffffff;
	return Crc;
}
// -------

bool CRCCheck(frame &ethernetFrame) {
	unsigned int dataLen = ADDRESS_SIZE + ADDRESS_SIZE + TYPE_SIZE + ethernetFrame.data.size();
	unsigned char *frameContent = new unsigned char[dataLen];
	unsigned int i;
	for (i = 0; i < ADDRESS_SIZE; i++) {
		frameContent[i] = ethernetFrame.da[i];
	}
	for (i = 0; i < ADDRESS_SIZE; i++) {
		frameContent[ADDRESS_SIZE + i] = ethernetFrame.sa[i];
	}
	for (i = 0; i < TYPE_SIZE; i++) {
		frameContent[ADDRESS_SIZE * 2 + i] = ethernetFrame.type[i];
	}
	for (i = 0; i < ethernetFrame.data.size(); i++) {
		frameContent[ADDRESS_SIZE * 2 + TYPE_SIZE + i] = ethernetFrame.data[i];
	}
	ethernetFrame.rr = GetCrc32((char*)frameContent, dataLen);
	unsigned int fcs = 0;
	for (size_t i = 0; i < FCS_SIZE; i++) {
		fcs <<= 8;
		fcs += ethernetFrame.fcs[i];
	}
	return fcs == ethernetFrame.rr;
}