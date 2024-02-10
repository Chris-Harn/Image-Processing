#include "Timer.h"

Timer::Timer( int targetFPS ) {
    m_targetFPS = targetFPS;
    m_deltaTime = 0.0;
    m_frames = 0;
    m_fps = 0;
    m_bRegulateFPS = false;

    m_frameDuration = std::chrono::milliseconds( int( 1000.0f / m_targetFPS ) );
    m_lastFrameTime = std::chrono::high_resolution_clock::now();
    m_lastSecondTime = m_lastFrameTime;
}

Timer::~Timer() {
    // Nothing allocated and nothing to release
}

void Timer::Tick() {
    auto now = std::chrono::high_resolution_clock::now();
    m_deltaTime = std::chrono::duration<double>( now - m_lastFrameTime ).count();
    m_lastFrameTime = now;

    m_frames++;

    if( std::chrono::duration<double>( now - m_lastSecondTime ).count() > 1.0 ) {
        m_fps = m_frames;
        m_frames = 0;
        m_lastSecondTime = now;
    }
    if( m_bRegulateFPS == true ) {
        // Adjust sleep time to maintain desired frame rate
        auto sleepTime = m_frameDuration - ( now - m_lastFrameTime );
        if( sleepTime > std::chrono::milliseconds( 0 ) ) {
            std::this_thread::sleep_for( sleepTime );
        }
    }
}

double Timer::GetDeltaTime() {
    return m_deltaTime * 1000.0;
}

void Timer::RegulateFPS( bool turnOnRegulator ) {
    m_bRegulateFPS = turnOnRegulator;
}

int Timer::GetFPS() {
    return m_fps;
}

bool Timer::GetRegulationValue() {
    return m_bRegulateFPS;
}