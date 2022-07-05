#ifndef __APPLICATION_H__
#define __APPLICATION_H__

class Application {
public:
    Application();

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
    bool AppRunning;
};

#endif

