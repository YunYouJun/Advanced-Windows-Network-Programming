#pragma once
#include <fstream>


#define TCP_SRV_PORT 2121
//#define UDP_SRV_PORT 2020 // pdf is error should be 2120

static const char *ponit[] = { "\x20\x20", "\xA8\x87", "\xA8\x86", "\xA8\x84", "\xA8\x83", "\xA8\x80" };//  ¨‡¨†¨…¨‚¨€

class Common {
public:
    // crc16
    static unsigned short CalculateCheckSum(const char *pdata, int length)
    {
        unsigned short *ptemp = (unsigned short *)pdata;
        unsigned long temp = 0;
        for (int i = 0; i < length / 2; i++)
        {
            temp += *ptemp++;
        }
        return ~((unsigned short)((temp & 0xFFFF) + (temp >> 16)));
    }

    // progress bar
    static void progress_bar(int per)
    {
        int i = 0;
        int num0 = 0;
        printf("\r  [");
        num0 = (100 - per) / 5;
        i = per;
        while (i > 5)
        {
            printf("%s", ponit[5]);
            i -= 5;
        }
        if (i)
        {
            printf("%s", ponit[i]);
        }
        for (i = 0; i < num0; i++)
        {
            printf("%s", ponit[0]);
        }
        printf("] %2d%%", per);
        if (per == 100)
        {
            printf("\n");
        }
    }
};