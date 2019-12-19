#include"stdafx.h"
#include"RTSP.h"
RTSP::RTSP()
{
    this->UserAgent = "User-Agent: LibVLC/2.1.5 (LIVE555 Streaming Media v2014.05.27) YunYouJun 2019.12.21";
    //this->ClientRtpPort = "56789";
    //this->ClientRtcpPort = "56790";
    this->request = "";
    this->response = "";
}

RTSP::~RTSP(){}

void RTSP::SetUrl(string url)
{
    this->url = url;
}

void RTSP::addCseq()
{
    int temp = atoi(Cseq.c_str());
    temp++;
    char s[10];
    _itoa_s(temp, s, 10);
    this->Cseq = string(s);
}

void RTSP::RequestOptions()
{
    addCseq();
    request = "OPTIONS " + url + " RTSP/1.0\r\n" + "CSeq: " + Cseq + "\r\n" + UserAgent + "\r\n\r\n";

}

void RTSP::RequestDescribe()
{
    addCseq();
    request = "DESCRIBE " + url + " RTSP/1.0\r\n" + "CSeq: " + Cseq + "\r\n" + "Accept: application/sdp\r\n" + UserAgent + "\r\n\r\n";
}

void RTSP::RequestSetup()
{
    addCseq();
    //request = "SETUP " + url + " RTSP/1.0\r\n" + "CSeq: " + Cseq + "\r\n" + "Transport: RTP/AVP;unicast;client_port=" + ClientRtpPort + "-" + ClientRtcpPort + "\r\n" + UserAgent + "\r\n\r\n";
    request = "SETUP " + url + " RTSP/1.0\r\n" + "CSeq: " + Cseq + "\r\n" + "Transport: RTP/AVP/TCP;interleaved=0-1" + "\r\n" + UserAgent + "\r\n\r\n";
}

void RTSP::RequestPlay(libvlc_time_t pos, string end)
{
    addCseq();
    if (pos == -1)
    {
        request = "PLAY " + url + " RTSP/1.0\r\n" +
            "CSeq: " + Cseq + "\r\n" + "Session: " + Session + "\r\n" + UserAgent + "\r\n\r\n";
    }
    else
    {
        char s[10];
        _itoa_s(pos, s, 10);
        string PosCSr = string(s);
        request = "PLAY " + url + " RTSP/1.0\r\n" + "CSeq: " + Cseq + "\r\n" + UserAgent + "\r\nSession: " + Session + "\r\nRange: npt=" + PosCSr + ".000-" + end + "\r\n\r\n";
    }
}

void RTSP::RequestPause()
{
    addCseq();
    request = "PAUSE " + url + " RTSP/1.0\r\n" + "CSeq: " + Cseq + "\r\n" + "Session: " + Session + "\r\n" + UserAgent + "\r\n\r\n";
}

void RTSP::RequestGetparameter()
{
    addCseq();
    request = "GET_PARAMETER " + url + " RTSP/1.0\r\n" + "CSeq: " + Cseq + "\r\n" + "Session: " + Session + "\r\n" + UserAgent + "\r\n\r\n";
}

void RTSP::RequestTeardown()
{
    addCseq();
    request = "TEARDOWN " + url + " RTSP/1.0\r\n" + "CSeq: " + Cseq + "\r\n" + "Session: " + Session + "\r\n" + UserAgent + "\r\n\r\n";
}

string RTSP::GetResponseSession()
{
    int beg = response.find("Session");
    int end = response.find("\r\n", beg);
    Session = response.substr(beg + strlen("Session: "), 8);
    // ∑Ò‘Úª·Ωÿ»° timeout
    return Session;
}

string RTSP::GetResponseCseq()
{
    int beg = response.find("CSeq");
    if (beg != string::npos) {
        int end = response.find("\r\n", beg);
        Cseq = response.substr(beg + strlen("CSeq: "), end - beg - strlen("CSeq: "));
        return Cseq;
    }
    else {
        return "";
    }
}

int RTSP::GetResponseStatus()
{
    int beg = response.find(' ');
    int end = response.find(' ', beg + 1);
    Status = atoi(response.substr(beg + 1, end - beg - 1).c_str());
    return Status;
}

void RTSP::GetResponseRtpPort()
{
    int beg = response.find("server_port");
    beg += strlen("server_port=");
    int end = response.find("-", beg);
    ServerRtpPort = response.substr(beg, end - beg);
}

void RTSP::SetRequestSession()
{
    Session = GetResponseSession();
}

void RTSP::GetResponseRtcpPort()
{
    int beg = response.find("server_port=" + ServerRtpPort + "-");
    beg += strlen("-") + strlen("server_port=") + ServerRtpPort.size();
    int end = response.find("\r\n", beg);
    ServerRtcpPort = response.substr(beg, end - beg);
}

bool RTSP::CheckRtspResponse(int type)
{
    GetResponseCseq();
    GetResponseStatus();
    if (Status == 200) {
        switch (type)
        {
        case 3:
            GetResponseSession();
            break;
        default:
            break;
        }
        return TRUE;
    }
    else {
        char s[4];
        _itoa_s(Status, s, 10);
        string error = "Error Status: " + string(s) + ". ";
        switch (Status)
        {
        case 405:
            error += "Method Not Allowed";
            break;
        case 451:
            error += "Parameter Not Understood";
            break;
        case 454:
            error += "Session Not Found";
            break;
        case 457:
            error += "Invalid Range";
            break;
        default:
            break;
        }
        CString c_error = CA2T(error.c_str());
        AfxMessageBox(c_error);
        return FALSE;
    }
}

void RTSP::GetAudioLen()
{
    int beg = response.find("a=range:npt=0-");
    beg += strlen("a=range:npt=0-");
    int end = response.find("\r\n", beg);
    fileLength = response.substr(beg, end - beg);
}