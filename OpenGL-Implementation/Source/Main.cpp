#include <iostream> // cout, endl

#include "Application.h"

int main()
{
    std::cout << "Program started without issue." << std::endl;

    Application mainApp;
    if( mainApp.Initialization( 600, 480, "Image Processing digital, VHS videos." ) ) {
        if(mainApp.AppStillOpen()) {
            mainApp.ProcessInput();
            mainApp.Update();
            mainApp.Render();
            std::cout << "Program finished one frame." << std::endl;
        }
    }

    mainApp.CleanUp();

    std::cout << "Program finished without issue." << std::endl;
}