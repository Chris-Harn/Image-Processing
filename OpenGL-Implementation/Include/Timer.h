#ifndef __TIMER_H__
#define __TIMER_H__

#include <chrono>
#include <thread>

class Timer {
public:
    Timer( int targetFPS = 24 );
    ~Timer();
    void Tick();
    double GetDeltaTime();
    void RegulateFPS( bool turnOnRegulator );
    int GetFPS();
    bool GetRegulationValue();
private:
    int m_targetFPS;
    std::chrono::milliseconds m_frameDuration;
    std::chrono::high_resolution_clock::time_point m_lastFrameTime;
    std::chrono::high_resolution_clock::time_point m_lastSecondTime;
    double m_deltaTime;
    int m_frames;
    int m_fps;
    bool m_bRegulateFPS;
};

#endif

