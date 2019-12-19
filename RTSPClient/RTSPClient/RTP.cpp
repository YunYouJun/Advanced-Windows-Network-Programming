#include "stdafx.h"
#include "RTP.h"

RTP::RTP(){}

// header 16
// https://datatracker.ietf.org/doc/rfc3550/?include_text=1
// 5.1 RTP Fixed Header Fields

RTP::RTP(char* buffer, int len)
{
    memset(RTPHeader, 0, sizeof(RTPHeader));
    memcpy(RTPHeader, buffer, 16);
    RTPLoad = new char[len - 16];
    memcpy(RTPLoad, buffer + 16, len - 16);
    Length = len - 16;
}

RTP::~RTP(){}
