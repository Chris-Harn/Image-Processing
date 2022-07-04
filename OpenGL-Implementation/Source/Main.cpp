

#include "Application.h"

int main()
{
    Application mainApp;
    if( mainApp.Initialization( 720, 480, "Image Processing digital, VHS videos." ) ) {
        while( mainApp.ContinueProgram() ) {
            mainApp.ProcessInput();
            mainApp.Update();
            mainApp.Render();
        }
    }

    mainApp.CleanUp();

    return 0;
}