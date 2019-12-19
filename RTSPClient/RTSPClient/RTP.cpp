#include "stdafx.h"
#include "RTP.h"

RTP::RTP(){}
RTP::RTP(char* buffer, int len)
{
    memset(RTPHeader, 0, sizeof(RTPHeader));
    memcpy(RTPHeader, buffer, 16);
    RTPLoad = new char[len - 16];
    memcpy(RTPLoad, buffer + 16, len - 16);
    Length = len - 16;
}

RTP::~RTP(){}
