#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

#define PREAMBLE_SIZE 7
#define ADDRESS_SIZE 6
#define TYPE_SIZE 2
#define FCS_SIZE 4

long long fileEnd;

struct frame {
	unsigned int no;	// Frame#
	unsigned int preamble[8];
	unsigned int start;
	unsigned int da[6];	// destination address
	unsigned int sa[6];	// source address
	unsigned int type[2];	// type
	string data;	// data
	unsigned int fcs[4];	// frame check sum
	//bool crc32;	// crc32
};

bool IsFrameStart(ifstream &fin);
frame ParseFrame(ifstream &fin, unsigned int no);
void PrintFrame(frame ethernetFrame);

void ReadFrame(string frameFilename) {
	ifstream fin;
	fin.open(frameFilename, ios::binary);

	if (!fin.is_open())
	{
		cout << "Failed to open file, please check if file is exist or not." << endl;
		return;
	}

	fin.seekg(0, ios::end);
	fileEnd = fin.tellg();
	fin.seekg(0, ios::beg);

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
	string data = "";

	ethernetFrame.no = no;
	for (int i = 0; i < PREAMBLE_SIZE; i++) {
		ethernetFrame.preamble[i] = 0xAA;
	}
	ethernetFrame.start = 0xAB;
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
			data += fin.get();
		}
	}
	else {
		fin.seekg(dataStart, ios::beg);
		for (int i = 0; i < fileEnd - dataStart - FCS_SIZE; i++) {
			data += fin.get();
		}
	}
	ethernetFrame.data = data;
	for (int i = FCS_SIZE - 1; i >= 0; i--) {
		ethernetFrame.fcs[i] = fin.get();
	}
	return ethernetFrame;
}

void PrintFrame(frame ethernetFrame) {
	cout << "Frame#:" << "\t\t\t" << ethernetFrame.no << endl;
	cout << "Preamble:" << "\t\t";
	for (int i = 0; i < PREAMBLE_SIZE; i++) {
		cout << hex << uppercase << ethernetFrame.preamble[i] << " ";
	}
	cout << endl;
	cout << "Start:" << "\t\t\t" << hex << uppercase << ethernetFrame.start << endl;
	cout << "Destination Address:" << "\t";
	for (int i = 0; i < ADDRESS_SIZE; i++) {
		cout << setw(2) << setfill('0') << hex << uppercase << ethernetFrame.da[i];
		if (i != ADDRESS_SIZE - 1) {
			cout << "-";
		}
		else {
			cout << endl;
		}
	}
	cout << "Source Address:" << "\t\t";
	for (int i = 0; i < ADDRESS_SIZE; i++) {
		cout << setw(2) << setfill('0') << hex << uppercase << ethernetFrame.sa[i];
		if (i != ADDRESS_SIZE - 1) {
			cout << "-";
		}
		else {
			cout << endl;
		}
	}
	cout << "Type:" << "\t\t\t";
	for (int i = 0; i < TYPE_SIZE; i++) {
		cout << setw(2) << setfill('0') << hex << uppercase << ethernetFrame.type[i];
		if (i != TYPE_SIZE - 1) {
			cout << " ";
		}
		else {
			cout << endl;
		}
	}
	cout << "Data:" << "\t\t\t" << ethernetFrame.data << endl;
	cout << "FCS:" << "\t\t\t";
	cout << "0x ";
	for (int i = 0; i < FCS_SIZE; i++) {
		cout << hex << nouppercase << ethernetFrame.fcs[i];
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
int main(int argc, char *argv[]) {
	string frameFilename;
	cout << "Frame Parser:" << endl;
	cout << "-----------------" << endl;
	cout << "Please input frame filename: ";
	//cin >> frameFilename;
	cout << "\n----------------------------------\n" << endl;
	if (argv[1]) {
		frameFilename = argv[1];
	}
	frameFilename = "testFrame";
	ReadFrame(frameFilename);
	return 0;
}