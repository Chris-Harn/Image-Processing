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
    static VideoPlayer *Instance() {
        if( s_pInstance == 0 ) {
            s_pInstance = new VideoPlayer();
        }
        return s_pInstance;
    }
    
    void ReadyCommand();
    void PlayCommand();
    void PauseCommand();
    void IncrementCommand();
    bool CurrentlyPlaying();
    bool CurrentlyPaused();
    bool CurrentlyIncrement();

public:
    VideoPlayer();
    ~VideoPlayer();

    static VideoPlayer *s_pInstance;

    VpState  m_VpState;
};

typedef VideoPlayer TheVideoPlayer;

#endif
