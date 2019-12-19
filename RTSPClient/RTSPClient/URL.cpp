#include "stdafx.h"
#include "URL.h"

URL::URL(){
    m_ip = "";
    m_name = "";
    m_port = "554";
    m_protocol = "";
}

URL::~URL(){}

// rtsp://computing.cuc.edu.cn:8554/Angel.mp3
// rtsp://rtsp.yunyoujun.cn/test.mp3
// rtsp://rtsp.yunyoujun.cn/adamas.mp3
// rtsp://rtsp.yunyoujun.cn/before-my-body-is-dry.mp3
// rtsp://rtsp.yunyoujun.cn/not-afraid.mp3
// rtsp://rtsp.yunyoujun.cn/overfly.mp3
// rtsp://rtsp.yunyoujun.cn/oyasumi.mp3
// rtsp://rtsp.yunyoujun.cn/riya.mp3
// rtsp://rtsp.yunyoujun.cn/will.mp3

void URL::read(string url) {
    int beg, end, pos;

    // set protocol
    beg = url.find("://");
    if (beg == string::npos) {
        return;
    }
    m_protocol = url.substr(0, beg);

    // set ip
    beg = strlen("rtsp://");
    end = url.find("/", beg);
    if (end == string::npos) {
        return;
    }
    m_ip = url.substr(beg, end - beg);
    pos = m_ip.find(":");
    // npos -1 4294967295
    if (pos != string::npos)
    {
        m_ip = m_ip.substr(0, pos);
    }
    if (isalpha(m_ip[0]))
    {
        string s(domainToIp(m_ip.c_str()));
        m_ip = s;
    }

    // set name
    beg = url.find("://");
    beg += strlen("://");
    end = url.find("/", beg);
    ++end;
    m_name = url.substr(end, url.size() - end);

    // set port
    beg = url.find_last_of(":");
    beg = beg + 1;
    end = url.find_last_of("/");
    // too long
    if ((end - beg) > 6)
    {
        m_port = "554";
    }
    else
    {
        m_port = url.substr(beg, end - beg);
    }
}

string URL::getIp()
{
    return m_ip;
}

string URL::getName()
{
    return m_name;
}

string URL::getPort()
{
    return m_port;
}

string URL::getProtocol() {
    return m_protocol;
}

char* URL::domainToIp(const char* domain)
{
    WSADATA  wsaData;
    int iResult;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup failed:%d\n", iResult);
        return NULL;
    }
    hostent *host = gethostbyname(domain);
    if (host != NULL)
    {
        int i = 0;
        in_addr addr;
        while (host->h_addr_list[i] != 0)
        {
            addr.S_un.S_addr = *(u_long*)host->h_addr_list[i++];
            return inet_ntoa(addr);
        }
    }
    WSACleanup();
    return NULL;
}