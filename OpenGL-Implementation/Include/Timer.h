#ifndef _TIMER_H_
#define _TIMER_H_

#include <chrono>

class Timer {
public:
    Timer();
    ~Timer();
    
    void Start( float targetFrames = 24.0f );
    float GetDeltaTicks(); // returns in milliseconds
    float GetiTime();
    void ResetiTime();
    void RestrictFrameRate();
private:
    // iTime Calculation
    std::chrono::time_point<std::chrono::steady_clock>  m_StartTime;
    std::chrono::time_point<std::chrono::steady_clock>  m_CurrentTime;

    // Restrict the Frame Rate Variables
    std::chrono::time_point<std::chrono::steady_clock>  m_LastFrameTime;
    std::chrono::time_point<std::chrono::steady_clock>  m_CurrentFrameTime;
    std::chrono::duration<float> m_DeltaFrameTime;

    float m_HoldToThisFrameRate;
};

#endif