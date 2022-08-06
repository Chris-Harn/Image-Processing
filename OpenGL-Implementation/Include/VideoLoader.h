#ifndef __VIDEOLOADER_H__
#define __VIDEOLOADER_H__

#include <stdio.h> // FILE, _popen, _pclose
#include <GL/glew.h>

#include "Defines.h"

class VideoLoader {
public:
    VideoLoader();
    ~VideoLoader();
    void LoadNewVideo( const char *filepath, const char *cmdFFmpeg );
    void GrabFrameFromVideo();
    void BindTexture( unsigned int slot = 0 );
    void UnbindTexture( unsigned int slot = 0 );
    void CleanUp();
public:
    FILE *m_pFFmpeg;
    int m_width;
    int m_height;

    // PBO
    int m_index;
    int m_nextIndex;
    unsigned int m_dataSize;
    unsigned int m_pboIDs[PBO_COUNT];
    GLuint m_textureID;
    GLubyte *m_pImageData;
};

#endif