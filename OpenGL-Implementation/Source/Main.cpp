#include "Application.h"

enum PROTO {
    WINDOW_WIDTH = 640,
    WINDOW_HEIGHT = 480,
    UPSCALE_WIDTH = 1280,
    UPSCALE_HEIGHT = 960,
    TARGET_FPS = 30,
};

int main()
{
    if( TheApplication::Instance()->Initialization( WINDOW_WIDTH, WINDOW_HEIGHT, UPSCALE_WIDTH, UPSCALE_HEIGHT, TARGET_FPS, "Image Processing Digital, VHS Videos" ) ) {
        while( TheApplication::Instance()->ContinueProgram() ) {
            TheApplication::Instance()->ProcessInput();
            TheApplication::Instance()->Update();
            TheApplication::Instance()->Render();
        }
    }

    TheApplication::Instance()->CleanUp();

    return 0;
}