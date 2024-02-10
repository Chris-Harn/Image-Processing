#ifndef __APPLICATION_H__
#define __APPLICATION_H__

class Window;
class Quad;
class TextRenderer;
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
    Window *g_pMainWindow;
    Window *g_pSecondaryWindow;
    Window *g_pGUIWindow;
    Quad *g_pQuad;
    Quad *g_pQuad2;
    TextRenderer *g_pTextRenderer;
    GUI *g_pGUI;
    Timer *g_pAppTimer;
    VideoLoader *g_pVideoLoader;
    VideoPlayer *g_pVideoPlayer;
};

typedef Application TheApplication;

#endif

