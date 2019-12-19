#include"stdafx.h"
#include"VLCPlayer.h"

// instance
VLCPlayer::VLCPlayer()
{
    vlcInstance = libvlc_new(0, NULL);
    vlcMediaPlayer = libvlc_media_player_new(vlcInstance);
}

VLCPlayer::~VLCPlayer()
{
    libvlc_media_player_release(vlcMediaPlayer);
    libvlc_release(vlcInstance);
}

void VLCPlayer::OpenMedia(const char* mediaPath)
{
    vlcMedia = libvlc_media_new_path(vlcInstance, mediaPath);
    libvlc_media_player_set_media(vlcMediaPlayer, vlcMedia);
}

void VLCPlayer::Play()
{
    libvlc_media_player_play(vlcMediaPlayer);
}

void VLCPlayer::Pause()
{
    libvlc_media_player_pause(vlcMediaPlayer);
}

void VLCPlayer::Stop()
{
    libvlc_media_player_stop(vlcMediaPlayer);
}

void VLCPlayer::SetTime(libvlc_time_t newTime)
{
    libvlc_media_player_set_time(vlcMediaPlayer, newTime);
}

libvlc_time_t VLCPlayer::GetLength()
{
    // libvlc_media_player_get_length return int64_t
    // https://www.videolan.org/developers/vlc/doc/doxygen/html/group__libvlc__media__player.html#ga057b0d82fb84f60ee24a6d76c1f135f3
    libvlc_time_t length;
    do {
        length = libvlc_media_player_get_length(vlcMediaPlayer);
    } while (length == 0);
    return length;
}

libvlc_time_t VLCPlayer::GetPlayTime()
{
    return libvlc_media_player_get_time(vlcMediaPlayer);
}

void VLCPlayer::SetVolume(int volume)
{
    libvlc_audio_set_volume(vlcMediaPlayer, volume);
}
