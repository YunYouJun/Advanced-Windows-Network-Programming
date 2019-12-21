#pragma once
#include <stdio.h>
#include <string>
#include <afxinet.h>
#include <iostream>
#include <fstream>
#include "vlc/vlc.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#define MAX_BUF_LEN 2048
#define DEFAULT_PATHLEN 64
#define MAXCLIENTS 5
#define MAXSOCKETS 10

using namespace std;

class URL
{
private:
    string m_ip;
    string m_name;
    string m_port;
    string m_protocol;

public:
    URL();
    ~URL();
    void read(string url);
	string getIp();
    string getName();
	int getPort();
    string getProtocol();
    char* domainToIp(const char* domain);
};

