#include "Application.h"

enum PROTO {
    WINDOW_WIDTH = 640,
    WINDOW_HEIGHT = 480,
    TARGET_FPS = 24,
};

int main()
{
    // Load in video and fill in x res, y res, frame rate, etc etc needed for
    // the application that follows

    Application MainApp;
    if( MainApp.Initialization( WINDOW_WIDTH, WINDOW_HEIGHT, TARGET_FPS, "Image Processing Digital, VHS Videos" ) ) {
        while( MainApp.ContinueProgram() ) {
            MainApp.ProcessInput();
            MainApp.Update();
            MainApp.Render();
        }
    }

    MainApp.CleanUp();

    return 0;
}