#ifndef __VIDEOPLAYER_H__
#define __VIDEOPLAYER_H__

enum VpState {
    WAITING = 0, // No video loaded
    READY, // Video loaded
    PAUSE, // Video loaded but paused
    PLAY, // Play the video
    INCREMENT // Increment 1 frame
};

class VideoPlayer {
public:
    VideoPlayer();
    ~VideoPlayer();

    void ReadyCommand();
    void PlayCommand();
    void PauseCommand();
    void IncrementCommand();
    bool CurrentlyPlaying();
    bool CurrentlyPaused();
    bool CurrentlyIncrement();

public:
    VpState  m_VpState;
};

#endif
