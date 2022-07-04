#ifndef __APPLICATION_H__
#define __APPLICATION_H__

class Application {
public:
    Application();

    bool Initialization( unsigned window_width, unsigned int window_height, const char *title );
    void ProcessInput();
    void Update();
    void Render();
    void CleanUp();

    bool AppStillOpen();

public:
    bool AppRunning;
};

#endif

