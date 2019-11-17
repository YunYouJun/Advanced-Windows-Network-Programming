// FileServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#include "../CBlockingSocket.h"
#include <fstream>
#include <process.h>

unsigned __stdcall tcpEchod(LPVOID pArguments)
{
	CBlockingSocket *cs = (CBlockingSocket *)pArguments;
	// use local variables to save the parameters passed by the incoming addresss of the thread to prevent the socket from being overwritten in the main thread
	CBlockingSocket ClientSocket = *cs;

	char filename[FPL] = { 0 };
	ClientSocket.Read(filename, sizeof(filename));
	cout << "  File requested from the client: " << filename << endl;

	fstream fs;
	fs.open(filename, ios::in | ios::binary);
	if (!fs)
	{
		cout << "  " << filename << " does not exist." << endl;
	}
	else
	{
		fs.seekg(0, ios::end);
		long long filelen = fs.tellg();
		fs.seekg(0, ios::beg);

		char buffer[BDP] = { 0 };
		if (filelen <= BDP)
		{
			fs.read(buffer, filelen);
			ClientSocket.Send(buffer, filelen);
		}
		else
		{
			long long temp = filelen;
			while (temp > BDP)
			{
				fs.read(buffer, BDP);
				ClientSocket.Send(buffer, BDP);
				temp -= BDP;
			}
			fs.read(buffer, temp);
			ClientSocket.Send(buffer, temp);
		}
		cout << "  Sent file back to the client: " << filename << endl;
	}

	ClientSocket.Close();
	return 0;
}

int main(int argc, char *argv[])
{
	string port = "8888";

	if (argc == 2)
	{
		port = argv[1];
	}
	else if (argc > 2){
		cout << "Please input like this: FileServer 8888." << endl;
		return -1;
	}

	CBlockingSocket ListenSocket, ClientSocket;

	CBlockingSocket::Initialize();
	ListenSocket.HintsAndResult(NULL, port.c_str());
	ListenSocket.Open();
	ListenSocket.Bind();
	ListenSocket.setSendRecvBuffer(BDP);
	ListenSocket.Listen();

	char name[256];
	gethostname(name, sizeof(name));
	hostent *remoteHost = gethostbyname(name);
	struct in_addr addr;
	addr.s_addr = *(u_long *)remoteHost->h_addr_list[0];
	// I want to use InetNtop but something wrong, and use InetNtop in other place.
	cout << "FileServer is listening on: " << inet_ntoa(addr) << ":" << port.c_str() << endl;

	while (1)
	{
		cout << "Waiting for a connection…" << endl;

		struct sockaddr their_addr;
		struct sockaddr_in their_addrin;
		ClientSocket = ListenSocket.Accept(their_addr);
		memcpy(&their_addrin, &their_addr, sizeof(their_addr));
		char ipstr[INET6_ADDRSTRLEN];
		InetNtop(their_addrin.sin_family, (void*)&their_addrin.sin_addr, ipstr, sizeof(ipstr));
		cout << "Accepted connection from " << ipstr << endl;

		HANDLE hThread;
		unsigned threadID;

		// 揭秘！为何要用_beginthreadex，而不用CreateThread和_beginthread
		// https://msd.misuland.com/pd/2884250137616455570
		hThread = (HANDLE)_beginthreadex(NULL, 0, &tcpEchod, &ClientSocket, 0, &threadID);
	}

	ListenSocket.Close();
	CBlockingSocket::Cleanup();
	return 0;
}