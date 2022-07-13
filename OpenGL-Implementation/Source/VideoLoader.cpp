#include "VideoLoader.h"

#include <fstream>
#include <string>

VideoLoader::VideoLoader() {
    m_pFFmpeg = nullptr;
    m_dataSize = 0;
    m_width = 0;
    m_height = 0;
}

VideoLoader::~VideoLoader() {

}

void VideoLoader::LoadNewVideo( const char *filepath, const char *cmdFFmpeg ) {
    std::ifstream f;
    f.open( filepath, f.binary | f.in );
    f.seekg( 64, f.beg );
    f.read( (char *)&m_width,  4 );
    f.read( (char *)&m_height, 4 );
    f.close();

    // X resolution * Y resolution * 3 color channels
    m_dataSize = m_width * m_height * 3;

    std::string input = "ffmpeg -i " + std::string( filepath ) + " " + std::string( cmdFFmpeg );

    m_pFFmpeg = _popen( input.c_str(), "rb" );
}

void VideoLoader::GrabFrameFromVideo() {
    //fread( ptr, 1, m_dataSize, m_pFFmpeg );
}

void VideoLoader::CleanUp() {
        fflush( m_pFFmpeg );
        _pclose( m_pFFmpeg );
}