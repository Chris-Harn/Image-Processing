#ifndef __APPLICATION_H__
#define __APPLICATION_H__

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
};

typedef Application TheApplication;

#endif

