#include <fstream>
#include <iostream>
using namespace std;

void ReadTestFrame()
{
	char data[100];
	ifstream infile;
	infile.open("test.txt");
	infile >> data;
	cout << data <<endl;
	infile >> data;
	cout << data << endl;

	infile.close();
}

int main()
{
	ReadTestFrame();
    std::cout << "Hello World!\n";
}