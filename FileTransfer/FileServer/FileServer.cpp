// FileServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "pch.h"
#include <fstream>
#include <iostream>

#include "../Message.h"
#include "../CBlockingSocket.h"
#include <process.h>
#include <io.h>
#include <vector>
#include <string>

#include "../UdpSocket.h"
#include "../Common.h"

struct ListenSocket {
	CBlockingSocket tcpSock;
	UdpSocket udpSock;
};

unsigned __stdcall tcpEchod(LPVOID pArguments)
{
	ListenSocket *cs = (ListenSocket *)pArguments;
	// use local variables to save the parameters passed by the incoming addresss of the thread to prevent the socket from being overwritten in the main thread
	ListenSocket listenSock = *cs;
	CBlockingSocket ClientSocket = listenSock.tcpSock;
	UdpSocket udpSock = listenSock.udpSock;

	// https://blog.csdn.net/max_cong/article/details/70155973
	// get client socket
	struct sockaddr_in c;
	socklen_t cLen = sizeof(c);
	getpeername(ClientSocket.m_socket, (struct sockaddr*) &c, &cLen); // ! use connectSock here.
	// get server socket
	struct sockaddr_in s;
	socklen_t sLen = sizeof(s);
	getsockname(ClientSocket.m_socket, (struct sockaddr*) &s, &sLen);
	string ip = inet_ntoa(s.sin_addr);

	int client_tcp_port = htons(c.sin_port);
	int client_udp_port = client_tcp_port + 1;
	// set udp client addr
	sockaddr_in udp_client_addr;
	udp_client_addr.sin_family = AF_INET;
	udp_client_addr.sin_port = htons(client_udp_port);
	udp_client_addr.sin_addr.S_un.S_addr = c.sin_addr.S_un.S_addr;

	while (1) {
		char bufCommand[MSG_HEADER_LENGTH + MAX_COMMAND_SIZE] = { 0 };
		if (ClientSocket.Recv(bufCommand, sizeof(bufCommand)) == -1) {
			return 0;
		}
		Message msg;
		msg.read(bufCommand);
		// get file list
		if (msg.getType() == COMMAND_DIR) {
			string file_list = "file-list.txt";
			cout << "  Get Command: dir" << endl;
			system("dir > file-list.txt");
			fstream fs;
			fs.open("file-list.txt", ios::in | ios::binary);
			fs.seekg(0, ios::end);
			long long filelen = fs.tellg();
			fs.seekg(0, ios::beg);
			char bufList[MAX_DATA_SIZE] = { 0 };
			fs.read(bufList, filelen);
			fs.close();
			ClientSocket.Send(bufList, MSG_HEADER_LENGTH + filelen);
			cout << "    Sent file list back to the client." << endl;
			remove(file_list.c_str());
		}
		else if (msg.getType() == COMMAND_DOWNLOAD) {
			// download
			cout << "  Get Command: download" << endl;
			char filename[MAX_COMMAND_SIZE] = { 0 };
			memcpy(filename, msg.getData(), msg.getSize());
			cout << "    File requested from the client: " << filename << endl;

            // for select
            fd_set fdread;
            timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = TIMEOUT_RESEND;

			fstream fs;
			fs.open(filename, ios::in | ios::binary);

            char bufSize[MSG_HEADER_LENGTH + MAX_COMMAND_SIZE] = { 0 };
            Message reply_msg;
            int filelen = -1;
            reply_msg.setType(FILE_DATA_SIZE);
            reply_msg.setSize(sizeof(filelen));
            if (!fs)
			{
				cout << "      " << filename << " does not exist." << endl;
                filelen = -1;
                // send file size back
                reply_msg.setSize(filelen);
                reply_msg.write(bufSize);
                ClientSocket.Send(bufSize, MSG_HEADER_LENGTH);
			}
			else
			{
				fs.seekg(0, ios::end);
				filelen = fs.tellg();
				fs.seekg(0, ios::beg);
                // send file size back
                reply_msg.setSize(filelen);
                reply_msg.write(bufSize);
                ClientSocket.Send(bufSize, MSG_HEADER_LENGTH);

				Message file_msg;
                cout << "    Sent file to the client(" << inet_ntoa(udp_client_addr.sin_addr) << ":" << htons(udp_client_addr.sin_port) << "): " << filename << endl;

				char bufData[MAX_DATA_SIZE] = { 0 };
				char buf[MAX_BUF_SIZE] = { 0 };
				char bufAck[MSG_HEADER_LENGTH] = { 0 };
				if (filelen <= MAX_DATA_SIZE)
				{
					fs.read(bufData, filelen);
                    file_msg.setType(FILE_DATA_END);
                    file_msg.setSeq(1);
					file_msg.setSize(filelen);
					file_msg.setData(bufData);
                    unsigned short data_checksum = Common::CalculateCheckSum(bufData, filelen);
                    file_msg.setChecksum(data_checksum);
					file_msg.write(buf);
					// send
					int send_b = sendto(udpSock.m_socket, buf, MSG_HEADER_LENGTH + file_msg.getSize(), 0, (sockaddr*)&udp_client_addr, sizeof(sockaddr));

					// stop and wait
                    while (true)
                    {
                        FD_ZERO(&fdread);
                        FD_SET(udpSock.m_socket, &fdread);
                        if (select(0, &fdread, NULL, NULL, &tv))
                        {
                            recvfrom(udpSock.m_socket, bufAck, MSG_HEADER_LENGTH, 0, (sockaddr*)&udp_client_addr, &sLen);
                            Message reply_msg;
                            reply_msg.read(bufAck);
                            if (reply_msg.getType() == FILE_DATA_ACK) {
                                break;
                            }
                        }
                        int send_b = sendto(udpSock.m_socket, buf, MSG_HEADER_LENGTH + file_msg.getSize(), 0, (sockaddr*)&udp_client_addr, sizeof(sockaddr));
                    }
                    cout << "      UDP: Send " << send_b << " B, data: " << file_msg.getSize() << " B." << endl;
				}
				else
				{
					int count = 0;
                    int block;
                    int totalBlock = (filelen / MAX_DATA_SIZE) + 1;
                    int percent = 0;
                    // set console color
                    HANDLE  hStdOutHandle;
                    WORD    wOldColorAttrs;
                    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
                    hStdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
                    GetConsoleScreenBufferInfo(hStdOutHandle, &csbiInfo);
                    wOldColorAttrs = csbiInfo.wAttributes;
					for (block = 1; block <= (filelen / MAX_DATA_SIZE); block++)
					{
						fs.read(bufData, MAX_DATA_SIZE);
                        file_msg.setType(FILE_DATA_REPLY);
                        file_msg.setSeq(block);
                        file_msg.setPosition(count);
                        file_msg.setSize(MAX_DATA_SIZE);
                        file_msg.setData(bufData);
                        unsigned short data_checksum = Common::CalculateCheckSum(bufData, MAX_DATA_SIZE);
                        file_msg.setChecksum(data_checksum);
						file_msg.write(buf);
                        int send_b = sendto(udpSock.m_socket, buf, MAX_BUF_SIZE, 0, (sockaddr*)&udp_client_addr, sizeof(sockaddr));

                        // avoid too fast
                        Sleep(1);
                        // stop and wait
                        while (true)
                        {
                            FD_ZERO(&fdread);
                            FD_SET(udpSock.m_socket, &fdread);
                            if (select(0, &fdread, NULL, NULL, &tv))
                            {
                                recvfrom(udpSock.m_socket, bufAck, MSG_HEADER_LENGTH, 0, (sockaddr*)&udp_client_addr, &sLen);
                                Message reply_msg;
                                reply_msg.read(bufAck);
                                if (reply_msg.getType() == FILE_DATA_ACK) {
                                    break;
                                }
                            }
                            //cout << "    Resend this packet." << endl;
                            int send_b = sendto(udpSock.m_socket, buf, MAX_BUF_SIZE, 0, (sockaddr*)&udp_client_addr, sizeof(sockaddr));
                        }
                        count += MAX_DATA_SIZE;
                        //cout << "      UDP: Send " << send_b << " B, data: " << file_msg.getSize() << " B." << endl;

                        // print progress bar
                        percent = 100 * file_msg.getSeq() / totalBlock;
                        Common::progress_bar(percent);
					}
                    // file data tail
					fs.read(bufData, filelen % MAX_DATA_SIZE);
                    file_msg.setType(FILE_DATA_END);
                    file_msg.setSeq(block);
					file_msg.setPosition(count);
					file_msg.setSize(filelen % MAX_DATA_SIZE);
					file_msg.setData(bufData);
                    WORD data_checksum = Common::CalculateCheckSum(bufData, filelen % MAX_DATA_SIZE);
                    file_msg.setChecksum(data_checksum);
					file_msg.write(buf);
                    int send_b = sendto(udpSock.m_socket, buf, MAX_BUF_SIZE, 0, (sockaddr*)&udp_client_addr, sizeof(sockaddr));

                    // select
                    while (true)
                    {
                        FD_ZERO(&fdread);
                        FD_SET(udpSock.m_socket, &fdread);
                        if (select(0, &fdread, NULL, NULL, &tv))
                        {
                            recvfrom(udpSock.m_socket, bufAck, MSG_HEADER_LENGTH, 0, (sockaddr*)&udp_client_addr, &sLen);
                            Message reply_msg;
                            reply_msg.read(bufAck);
                            if (reply_msg.getType() == FILE_DATA_ACK) {
                                break;
                            }
                        }
                        //cout << "    Resend this packet." << endl;
                        int send_b = sendto(udpSock.m_socket, buf, MAX_BUF_SIZE, 0, (sockaddr*)&udp_client_addr, sizeof(sockaddr));
                    }
                    //cout << "      UDP: Send " << send_b << " B, data: " << file_msg.getSize() << " B." << endl;
                    percent += 1;
                    SetConsoleTextAttribute(hStdOutHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                    Common::progress_bar(percent);
                    SetConsoleTextAttribute(hStdOutHandle, wOldColorAttrs);
				}
                cout << "    Send file " << filename << " completed." << endl;
			}
			fs.close();
		}
		else if (msg.getType() == COMMAND_EXIT) {
			cout << "  Get Command: exit" << endl;
			udpSock.Close();
			ClientSocket.Close();
			cout << "    Client exited." << endl;
			return 0;
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	u_short port = TCP_SRV_PORT;
	if (argc == 2)
	{
		port = atoi(argv[1]);
	}
	else if (argc > 2){
		cout << "Please input like this: FileServer 2121." << endl;
		return -1;
	}
	// first !
	CBlockingSocket::Initialize();

	CBlockingSocket ListenSocket(NULL, port);
	CBlockingSocket ClientSocket;
	UdpSocket udpSock;

	ListenSocket.Open();
	ListenSocket.Bind();
	ListenSocket.setSendRecvBuffer(MAX_DATA_SIZE);
	if (!ListenSocket.Listen()) {
		return -1;
	}
	struct sockaddr_in s;
	socklen_t sLen = sizeof(s);
	getsockname(ListenSocket.m_socket, (struct sockaddr*) &s, &sLen);
	cout << "FileServer(TCP) is listening on: " << ListenSocket.m_ip << ":" << ListenSocket.m_port << endl;
	// bind udp
	udpSock.Open();
	udpSock.Bind(NULL, port - 1);
	cout << "FileServer(UDP) is listening on: " << udpSock.m_ip << ":" << udpSock.m_port << endl;

	while (1)
	{
		struct sockaddr their_addr;
		struct sockaddr_in their_addrin;
		ClientSocket = ListenSocket.Accept(their_addr);
		memcpy(&their_addrin, &their_addr, sizeof(their_addr));
		char ipstr[INET6_ADDRSTRLEN];
		InetNtop(their_addrin.sin_family, (void*)&their_addrin.sin_addr, ipstr, sizeof(ipstr));
		cout << "Accepted connection from " << inet_ntoa(their_addrin.sin_addr) << ":" << htons(their_addrin.sin_port) << endl;

		struct ListenSocket listenSock = { ClientSocket, udpSock };
		HANDLE hThread;
		unsigned threadID;
		// 为何要用_beginthreadex，而不用CreateThread和_beginthread
		// https://blog.csdn.net/foreveyking/article/details/78029328
		hThread = (HANDLE)_beginthreadex(NULL, 0, &tcpEchod, &listenSock, 0, &threadID);
	}

	udpSock.Close();
	ListenSocket.Close();
	CBlockingSocket::Cleanup();
	return 0;
}