#pragma once
#include "vlc/vlc.h"
#include <string>

#pragma comment(lib, "libvlc.lib")
#pragma comment(lib, "libvlccore.lib")

using namespace std;

class VLCPlayer
{
    libvlc_instance_t *vlcInstance;
    libvlc_media_player_t *vlcMediaPlayer;
    libvlc_media_t *vlcMedia;
    int volume;
public:
    VLCPlayer();
    ~VLCPlayer();
    void OpenMedia(const char* mediaPath);
    libvlc_time_t GetPlayTime();
    libvlc_time_t GetLength();
    void Play();
    void Pause();
    void Stop();
    void SetVolume(int vol);
    void SetTime(libvlc_time_t newTime);
};