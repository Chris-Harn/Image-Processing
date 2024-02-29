#ifndef __APPLICATION_H__
#define __APPLICATION_H__

class Window;
class Quad;
class TextRenderer;
//#include "OpenGL/TextRenderer.h"
class GUI;
#include "Timer.h"
class VideoLoader;
class VideoPlayer;

class Application {
public:
    static Application *Instance() {
        if( s_pInstance == 0 ) {
            s_pInstance = new Application();
        }
        return s_pInstance;
    }

    bool Initialization( unsigned int window_width,
                         unsigned int window_height,
                         unsigned int upscale_width,
                         unsigned int upscale_height,
                         float video_fps,
                         const char *title );
    void ProcessInput();
    void Update();
    void Render();
    void CleanUp();

    bool ContinueProgram();

public:
    Application();
    ~Application() { }

    static Application *s_pInstance;

    bool AppRunning;

    // OpenGL Structures
    Window *m_pMainWindow;
    Window *m_pSecondaryWindow;
    Window *m_pGUIWindow;
    Quad *m_pQuad;
    Quad *m_pQuad2;
    TextRenderer *m_pTextRenderer;
    GUI *m_pGUI;
    Timer *m_pAppTimer;
    VideoLoader *m_pVideoLoader;
    VideoPlayer *m_pVideoPlayer;
};

typedef Application TheApplication;

#endif

