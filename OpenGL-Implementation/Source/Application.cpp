#include <iostream> // cout, endl

#include "Application.h"

//#include "OpenGL\Window.h"
//#include "OpenGL\Quad.h"
//
//Window *g_pMainWindow;
//Window *g_pSecondaryWindow;
//Quad *g_pQuad;

Application::Application() : AppRunning(true) {

}

bool Application::Initialization( unsigned window_width, unsigned int window_height, const char *title ) {
    //g_pMainWindow = new Window();
    //g_pSecondaryWindow = new Window();
    //g_pQuad = new Quad();

    std::cout << "Program started without issue." << std::endl;

    return true;
}

void Application::ProcessInput() {

}

void Application::Update() {
    AppRunning = false;
}

void Application::Render() {
    std::cout << "Program finished one frame." << std::endl;
}

void Application::CleanUp() {
    //if( g_pMainWindow != nullptr ) delete g_pMainWindow;
    //if( g_pSecondaryWindow != nullptr ) delete g_pSecondaryWindow;
    //if( g_pQuad != nullptr ) delete g_pQuad;

    std::cout << "Program finished without issue." << std::endl;
}

bool Application::AppStillOpen() {
    return AppRunning;
}