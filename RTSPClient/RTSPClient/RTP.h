#pragma once
#include "URL.h"
class RTP
{
public:
    RTP();
    RTP(char* buffer, int len);
    ~RTP();

    BYTE RTPHeader[16];
    char* RTPLoad;
    int Length;
};