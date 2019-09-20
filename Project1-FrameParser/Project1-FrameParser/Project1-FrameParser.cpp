#include <iostream>
#include <fstream>
using namespace std;

#define PREAMBLE_SIZE 7

struct frame {
	int no;	// Frame#
	int preamble[8];
	int start;
};

bool IsFrameStart(ifstream& file);
frame ParseFrame(long long start);
//frame ethernetFrame
void PrintFrame();

void ReadFrame(string frameFilename) {
	ifstream fin;
	fin.open(frameFilename, ios::binary);

	if (!fin.is_open())
	{
		cout << "Failed to open file, please check if file is exist or not." << endl;
		return;
	}

	int size;
	fin.seekg(0, ios::end);
	//size = fin.tellg();
	fin.seekg(0, ios::beg);

	//while (true) {
	//	ParseFrame(size);
	//}

	long long start;
	if (IsFrameStart(fin)) {
		start = fin.tellg();
		ParseFrame(start);
	}

	PrintFrame();

	int c;
	while (!fin.eof()) {
		c = fin.get();
		cout << c;
	}
	
	cout << "\nDone" << endl;
	fin.close();
}

frame ParseFrame(long long start, int i) {
	frame ethernetFrame = {
		i,
		{0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA},
		0xAB
	};
	return ethernetFrame;
}

//frame ethernetFrame
void PrintFrame() {
	cout << "Frame#:" << "\t\t\t" << endl;
	cout << "Preamble:" << "\t\t" << "AA AA AA AA AA AA AA" << endl;
	cout << "Start:" << "\t\t\t" << "AB" << endl;
}

bool IsFrameStart(ifstream &fin) {
	int p;
	// avoid redundance
	while (true) {
		for (int i = 0; i < PREAMBLE_SIZE; i++) {
			if (fin.eof()) return false;
			p = fin.get();
			if (p != 0xAA) {
				i = -1;	// keep seaching
			}
		}
		if (fin.eof()) return false;
		if (fin.get() == 0xAB) {
			return true;
		}
	}
	return false;
}

frame ParseFrame(long long start)
{
	return frame();
}

int main() {
	string frameFilename;
	cout << "Frame Parser:" << endl;
	cout << "------------" << endl;
	cout << "Input 'exit' to exit." << endl;
	//while (true) {
		cout << "Please input frame filename: ";
		//cin >> frameFilename;
		//if (frameFilename == "exit") {
		//	exit(0);
		//}

		// remember to remove
		frameFilename = "testFrame";

		ReadFrame(frameFilename);
	//}
	return 0;
}