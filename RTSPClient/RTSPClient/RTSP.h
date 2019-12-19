#pragma once
#include"ClientSocket.h"
#include<string>
#define OPTIONS 1
#define DESCRIBE 2
#define SETUP  3
#define PLAY 4
#define PAUSE 5
#define TEARDOWN 6
#define GET_PARAMETER 7

class RTSP
{
public:
    RTSP();
    ~RTSP();
    void SetUrl(string URL);
    void RequestOptions();
    void RequestDescribe();
    void RequestSetup();
    void RequestPlay(libvlc_time_t pos, string end = "");
    void RequestPause();
    void RequestGetparameter();
    void RequestTeardown();
    string GetResponseSession();
    string GetResponseCseq();
    int GetResponseStatus();
    void GetResponseRtpPort();
    void GetResponseRtcpPort();
    void SetRequestSession();
    bool CheckRtspResponse(int type);
    void addCseq();
    void GetAudioLen();

    string Session;
    string Cseq;
    string ServerRtpPort;
    string ServerRtcpPort;
    string request;
    string response;
    string url;
    string ClientRtpPort;
    string ClientRtcpPort;
    string UserAgent;
    int Status;
    string fileLength;
};
