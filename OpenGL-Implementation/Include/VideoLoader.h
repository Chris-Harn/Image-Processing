#ifndef __VIDEOLOADER_H__
#define __VIDEOLOADER_H__

#include <stdio.h> // FILE, _popen, _pclose

class VideoLoader {
public:
    VideoLoader();
    ~VideoLoader();
    void LoadNewVideo( const char *filepath, const char *cmdFFmpeg );
    void GrabFrameFromVideo();
    void CleanUp();
public:
    FILE *m_pFFmpeg;
    unsigned int m_dataSize;
    int m_width;
    int m_height;
};

#endif