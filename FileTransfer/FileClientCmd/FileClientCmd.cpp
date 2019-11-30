// FileClientCmd.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <fstream>
#include <iostream>

#include "../Common.h"
#include "../CBlockingSocket.h"
#include "../UdpSocket.h"
#include "../Message.h"
#include <string>
#include <process.h>
#include <windows.h>

unsigned __stdcall downloadFile(LPVOID pArguments) {

}

int main(int argc, char *argv[])
{
	argc = 2;
	argv[1] = (char*)"10.64.132.147";
	char *ip = argv[1];

	if (argc != 2)
	{
		cout << "Please input like this: FileClientCmd 10.64.132.147" << endl;
		return -1;
	}

	CBlockingSocket::Initialize();

	// init socket
	CBlockingSocket ConnectSocket(argv[1], TCP_SRV_PORT);
	ConnectSocket.Open();
	ConnectSocket.setSendRecvBuffer(MAX_BUF_SIZE);
	if (!ConnectSocket.Connect()) {
		cout << "Connect failed" << endl;
		return -1;
	}

	// get port
	struct sockaddr_in c;
	struct sockaddr_in s;
	socklen_t cLen = sizeof(c);
	socklen_t sLen = sizeof(s);
	getsockname(ConnectSocket.m_socket, (struct sockaddr*) &c, &cLen);
	getpeername(ConnectSocket.m_socket, (struct sockaddr*) &s, &sLen);
	cout << "Connection established to remote Server at " << argv[1] << ":" << htons(s.sin_port) << endl;

	while (TRUE) {
        cout << "-------------------------------------------------------------" << endl;
		cout << "Please input command(dir/download/exit): ";
		string command;
		cin >> command;
		if (command == "dir") {
			// get file list by command in tcp
			Message msg(COMMAND_DIR, MSG_HEADER_LENGTH, 0, 0, NULL);
			char buffer[MSG_HEADER_LENGTH + MAX_COMMAND_SIZE] = { 0 };
			msg.write(buffer);
			ConnectSocket.Send(buffer, MSG_HEADER_LENGTH);

			// get file list
			char bufList[MAX_DATA_SIZE] = { 0 };
			ConnectSocket.Recv(bufList, MAX_DATA_SIZE);
			cout << bufList << endl;
		}
		else if (command == "download") {
            //struct ListenSocket listenSock = { ClientSocket, udpSock };
            //HANDLE hThread;
            //unsigned threadID;
            //// 为何要用_beginthreadex，而不用CreateThread和_beginthread
            //// https://blog.csdn.net/foreveyking/article/details/78029328
            //hThread = (HANDLE)_beginthreadex(NULL, 0, &downloadFile, &listenSock, 0, &threadID);

            // recv by udp
            UdpSocket udpSock;
            udpSock.Open();
            int client_tcp_port = htons(c.sin_port);
            int client_udp_port = client_tcp_port + 1;
            int server_tcp_port = htons(s.sin_port);
            int server_udp_port = server_tcp_port - 1;
            udpSock.Bind(NULL, client_udp_port);

            // set udp_server_addr
            sockaddr_in udp_server_addr;
            udp_server_addr.sin_family = AF_INET;
            udp_server_addr.sin_port = htons(server_udp_port);
            udp_server_addr.sin_addr.S_un.S_addr = s.sin_addr.S_un.S_addr;

			char filename[MAX_COMMAND_SIZE] = { 0 };
			char savepath[MAX_COMMAND_SIZE] = { 0 };
			cout << "  Input file path: ";
			cin >> filename;
			cout << "  Input save path: ";
			cin >> savepath;

			// send file name & get file size
			Message msg;
			char buffer[MAX_COMMAND_SIZE] = { 0 };
			msg.setType(COMMAND_DOWNLOAD);
			msg.setSize(strlen(filename));
			msg.setData(filename);
			msg.write(buffer);
			ConnectSocket.Send(buffer, MSG_HEADER_LENGTH + strlen(filename));

            Message size_msg;
            ConnectSocket.Recv(buffer, MSG_HEADER_LENGTH);
            size_msg.read(buffer);
            int filelen = -1;
            if (size_msg.getType() == FILE_DATA_SIZE) {
                filelen = size_msg.getSize();
            }
            if (filelen == -1) {
                cout << "  File " << filename << " is not existed." << endl;
            }
            else {
                fstream fs;
                fs.open(savepath, ios::out | ios::binary);
                fs.close();
                fs.open(savepath, ios::app | ios::binary);

                int percent = 0;
                int block = filelen / MAX_DATA_SIZE + 1;
                char recvBuf[MAX_BUF_SIZE] = { 0 };
                unsigned long lastSeq = 0;
                int filerecv = 0;
                cout << "  Receiving file: " << filename << " （" << filelen << " B）" << endl;
                while (1)
                {
                    int sLen = sizeof(sockaddr);
                    filerecv = recvfrom(udpSock.m_socket, recvBuf, MAX_BUF_SIZE, 0, (sockaddr*)&udp_server_addr, &sLen);
                    if (filerecv == -1) {
                        break;
                    }
                    // check
                    Message file_msg;
                    file_msg.read(recvBuf);
                    if (file_msg.getChecksum() != Common::CalculateCheckSum(file_msg.getData(), file_msg.getSize())) {
                        cout << "    Checksum is wrong, drop it." << endl;
                        continue;
                    }

                    // send ack
                    Message data_ack_msg;
                    data_ack_msg.setType(FILE_DATA_ACK);
                    char bufAck[MSG_HEADER_LENGTH] = { 0 };
                    data_ack_msg.write(bufAck);

                    // get it before drop it
                    if (lastSeq == file_msg.getSeq()) {
                        //cout << "  Got it before, drop it." << endl;
                        //cout << "    Send ack back to the server." << endl;
                        sendto(udpSock.m_socket, bufAck, MSG_HEADER_LENGTH, 0, (sockaddr*)&udp_server_addr, sizeof(sockaddr));
                        continue;
                    }
                    fs.write(file_msg.getData(), file_msg.getSize());
                    lastSeq = file_msg.getSeq();
                    //cout << "    Send ack back to the server." << endl;
                    sendto(udpSock.m_socket, bufAck, MSG_HEADER_LENGTH, 0, (sockaddr*)&udp_server_addr, sizeof(sockaddr));
                    //cout << "    UDP: Received " << filerecv << " B, data: " << file_msg.getSize() << " B." << endl;


                    // set console color
                    HANDLE  hStdOutHandle;
                    WORD    wOldColorAttrs;
                    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
                    hStdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
                    GetConsoleScreenBufferInfo(hStdOutHandle, &csbiInfo);
                    wOldColorAttrs = csbiInfo.wAttributes;
                    
                    // print progress bar
                    percent = 100 * file_msg.getSeq() / block;
                    if (percent == 100) {
                        SetConsoleTextAttribute(hStdOutHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                        Common::progress_bar(percent);
                        SetConsoleTextAttribute(hStdOutHandle, wOldColorAttrs);
                    }
                    else {
                        Common::progress_bar(percent);
                    }

                    // over
                    if (file_msg.getType() == FILE_DATA_END) {
                        cout << "  Download file " << filename << " completed." << endl;
                        break;
                    }
                }
                fs.close();
            }
            udpSock.Close();
		}
		else if (command == "exit") {
			Message msg(COMMAND_EXIT, MSG_HEADER_LENGTH, 0, 0, NULL);
			char buffer[MSG_HEADER_LENGTH + MAX_COMMAND_SIZE] = { 0 };
			msg.write(buffer);
			ConnectSocket.Send(buffer, MSG_HEADER_LENGTH);

			ConnectSocket.Close();
			CBlockingSocket::Cleanup();
			return 0;
		}
	}

	ConnectSocket.Close();
	CBlockingSocket::Cleanup();
	return 0;
}