#include "Application.h"

int main()
{
    // Load in video and fill in x res, y res, frame rate, etc etc needed for
    // the application that follows

    Application mainApp;
    if( mainApp.Initialization( 640, 480, 24.0f, "Image Processing Digital, VHS Videos" ) ) {
        while( mainApp.ContinueProgram() ) {
            mainApp.ProcessInput();
            mainApp.Update();
            mainApp.Render();
        }
    }

    mainApp.CleanUp();

    return 0;
}