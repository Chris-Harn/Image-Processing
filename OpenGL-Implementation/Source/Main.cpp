

#include "Application.h"

int main()
{
    Application mainApp;
    if( mainApp.Initialization( 600, 480, "Image Processing digital, VHS videos." ) ) {
        if(mainApp.AppStillOpen()) {
            mainApp.ProcessInput();
            mainApp.Update();
            mainApp.Render();
        }
    }

    mainApp.CleanUp();
}