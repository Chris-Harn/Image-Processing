#include "VideoLoader.h"

#include <fstream>
#include <string>

VideoLoader::VideoLoader() {
    // Video
    m_pVFFmpeg = nullptr;
    m_width = 0;
    m_height = 0;

    // Audio 
    m_pAFFmpeg = nullptr;

    // PBO starts here. 
    m_index = 0;
    m_nextIndex = 0;
    // m_dataSize will change size when we get the actual video
    // size. This larger size is make sure the PBO(s) pass up
    // to a certain value to the GPU
    m_dataSize = X_RESOLUTION * Y_RESOLUTION * COLOR_CHANNELS;
    m_pImageData = nullptr;

    glGenTextures( 1, &m_textureID );
    glBindTexture( GL_TEXTURE_2D, m_textureID );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, X_RESOLUTION, Y_RESOLUTION, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *)m_pImageData );
    glBindTexture( GL_TEXTURE_2D, 0 );

    glGenBuffers( PBO_COUNT, m_pboIDs );
    for( int i = 0; i < PBO_COUNT; i++ ) {
        // GL_PIXEL_UNPACK_BUFFER means we're transfering pixel data from the application to OpenGL(GPU)
        glBindBuffer( GL_PIXEL_UNPACK_BUFFER, m_pboIDs[i] );
        glBufferData( GL_PIXEL_UNPACK_BUFFER, m_dataSize, 0, GL_STREAM_DRAW );
    }
    glBindBuffer( GL_PIXEL_UNPACK_BUFFER, 0 );
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

    // Updated dataSize to video's actual size
    m_dataSize = m_width * m_height * 3;

    std::string inputVid = "ffmpeg -i " + std::string( filepath ) + " " + std::string( cmdFFmpeg );

    m_pVFFmpeg = _popen( inputVid.c_str(), "rb" );

    if(SPEED_BUFFER == true ) {
        setvbuf( m_pVFFmpeg, NULL, _IOFBF, m_dataSize );
    }

    std::string inputAud = "ffmpeg -i " + std::string( filepath ) + " -f s16le -ac 1 - "; 
    m_pAFFmpeg = _popen( inputAud.c_str(), "r" );

    m_sample = new int();
}

bool VideoLoader::GrabFrameFromVideo() {
    m_index = ( m_index + 1 ) % PBO_COUNT;
    m_nextIndex = ( m_index + 1 ) % PBO_COUNT;

    glBindTexture( GL_TEXTURE_2D, m_textureID );
    glBindBuffer( GL_PIXEL_UNPACK_BUFFER, m_pboIDs[m_index] );

    // copy pixels from PBO to texture object
    // Use offset instead of ponter.
    glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, 0 );

    // Bind PBO to update pixel values
    glBindBuffer( GL_PIXEL_UNPACK_BUFFER, m_pboIDs[m_nextIndex] );

    // map the buffer object into client's memory
    // Note that glMapBuffer() causes sync issue.
    // If GPU is working with this buffer, glMapBuffer() will wait(stall)
    // for GPU to finish its job. To avoid waiting (stall), you can call
    // first glBufferData() with NULL pointer before glMapBuffer().
    // If you do that, the previous data in PBO will be discarded and
    // glMapBuffer() returns a new allocated pointer immediately
    // even if GPU is still working with the previous data.
    glBufferData( GL_PIXEL_UNPACK_BUFFER, m_dataSize, 0, GL_STREAM_DRAW );

    // map the buffer object into client's memory
    GLubyte *ptr = (GLubyte *)glMapBuffer( GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY );
    if( ptr ) {
        // Grab Video and put into PBO location space
        int count = fread( ptr, 1, m_dataSize, m_pVFFmpeg );

        // Grab Audio
        int aCount = fread( m_sample, 2, 2, m_pAFFmpeg );

        // Set flag deciding if we can turn off playback/recording
        if( count != m_dataSize ) {
            // Exit as video is finished
            return false;
        }

        glUnmapBuffer( GL_PIXEL_UNPACK_BUFFER );  // release pointer to mapping buffer
    }

    glBindBuffer( GL_PIXEL_UNPACK_BUFFER, 0 );
    glBindTexture( GL_TEXTURE_2D, 0 );

    return true;
}

int *VideoLoader::GrabAudioFromVideo() {
    return m_sample;
}

void VideoLoader::BindTexture( unsigned int slot ) {
    glActiveTexture( GL_TEXTURE0 + slot );
    glBindTexture( GL_TEXTURE_2D, m_textureID );
}

void VideoLoader::UnbindTexture( unsigned int slot ) {
    glBindTexture( GL_TEXTURE_2D, GL_TEXTURE0 + slot );
}

void VideoLoader::CleanUp() {
        // Video
        fflush( m_pVFFmpeg );
        _pclose( m_pVFFmpeg );

        // Audio
        fflush( m_pAFFmpeg );
        _pclose( m_pAFFmpeg );

        // Delete space reserved in heap
        if( m_pImageData != 0 ) {
            delete[] m_pImageData;
            m_pImageData = 0;
        }

        // Delete PBOs
        if( m_pboIDs[0] != 0 ) {
            glDeleteBuffers( PBO_COUNT, m_pboIDs );
        }

        // Delete Texture
        if( m_textureID != 0 ) {
            glDeleteTextures( 1, &m_textureID );
            m_textureID = 0;
        }
}