#include "VideoPlayer.h"

VideoPlayer *VideoPlayer::s_pInstance = 0;

VideoPlayer::VideoPlayer() {
    m_VpState = VpState::WAITING;
}

VideoPlayer::~VideoPlayer() {

}

void VideoPlayer::ReadyCommand() {
    if( m_VpState == VpState::WAITING ) {
        m_VpState = VpState::READY;
    }
}

void VideoPlayer::PlayCommand() {
    if( m_VpState != VpState::WAITING ) {
        m_VpState = VpState::PLAY;
    }
}

void VideoPlayer::PauseCommand() {
    if( m_VpState != VpState::WAITING ) {
        m_VpState = VpState::PAUSE;
    }
}

void VideoPlayer::IncrementCommand() {
    if( m_VpState != VpState::WAITING ) {
        m_VpState = VpState::INCREMENT;
    }
}

bool VideoPlayer::CurrentlyPlaying() {
    return ( m_VpState == VpState::PLAY );
}

bool VideoPlayer::CurrentlyPaused() {
    return ( m_VpState == VpState::PAUSE );
}

bool VideoPlayer::CurrentlyIncrement() {
    return ( m_VpState == VpState::INCREMENT );
}