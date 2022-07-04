#include <iostream> // cout, endl

#include "Application.h"

#include "OpenGL\Window.h"
#include "OpenGL\Quad.h"
#include "Timer.h"

Window *g_pMainWindow = nullptr;
Window *g_pSecondaryWindow = nullptr;
Quad *g_pQuad = nullptr;
Timer *g_pAppTimer;

Application::Application() : AppRunning(true) {

}

bool Application::Initialization( unsigned window_width, unsigned int window_height, float video_fps, const char *title ) {
    g_pMainWindow = new Window();
    if( g_pMainWindow->Initialization( window_width, window_height, title ) != true) {
        return false;
    }

    g_pSecondaryWindow = new Window();
    if( g_pSecondaryWindow->Initialization( window_width, window_height, "Original Video" ) != true ) {
        return false;
    }

    g_pQuad = new Quad();

    g_pAppTimer = new Timer();
    g_pAppTimer->Start( video_fps );

    std::cout << "Program started without issue." << std::endl;

    return true;
}

void Application::ProcessInput() {

}

void Application::Update() {
    //AppRunning = false;
}

void Application::Render() {
    g_pAppTimer->RestrictFrameRate();

    std::cout << "Program finished one frame." << std::endl;
}

void Application::CleanUp() {
    if( g_pMainWindow != nullptr ) delete g_pMainWindow;
    if( g_pSecondaryWindow != nullptr ) delete g_pSecondaryWindow;
    if( g_pQuad != nullptr ) delete g_pQuad;

    std::cout << "Program finished without issue." << std::endl;
}

bool Application::ContinueProgram() {
    return AppRunning;
}