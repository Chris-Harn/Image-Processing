#include "Timer.h"

#include <thread>

Timer::Timer() {
    // Variables for frame rate cap
    m_HoldToThisFrameRate = 0.0f;
}

Timer::~Timer() {

}

void Timer::Start( float targetFrames ) {
    m_StartTime = m_LastFrameTime = std::chrono::high_resolution_clock::now();

    // Target: 60 fps or 16.6666 ms/frame
    if( targetFrames > 0.0f ) {
        m_HoldToThisFrameRate = ( 1000.0f / targetFrames );
    } else {
        m_HoldToThisFrameRate = ( 1000.0f / 30.0f ); // 30 fps or 33.333 ms/frame  
    }
}

float Timer::GetDeltaTicks() {
    m_CurrentFrameTime = std::chrono::high_resolution_clock::now();
    m_DeltaFrameTime = m_CurrentFrameTime - m_LastFrameTime; // in seconds

    return (float)( m_DeltaFrameTime.count() * 1000.0f );
}

float Timer::GetiTime() {
    m_CurrentTime = std::chrono::high_resolution_clock::now();
    return ( ( m_CurrentTime - m_StartTime ).count() / 1000000000.0f );
}

void Timer::ResetiTime() {
    m_StartTime = std::chrono::high_resolution_clock::now();
}

void Timer::RestrictFrameRate() {
        while( GetDeltaTicks() < m_HoldToThisFrameRate ) {
            // Divide by 3.0 is magic number that tends to produce steady fps at the desired FPS Limit
            std::this_thread::sleep_for( std::chrono::milliseconds( int( ( m_HoldToThisFrameRate / 3.0 ) - GetDeltaTicks() ) ) );
        }
        m_LastFrameTime = std::chrono::high_resolution_clock::now();
}