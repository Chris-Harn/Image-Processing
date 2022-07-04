#include <iostream> // cout, endl

#include "Application.h"

#include "OpenGL\Window.h"
#include "OpenGL\Quad.h"

Window *g_pMainWindow;
Window *g_pSecondaryWindow;

Application::Application() : AppRunning(true) {

}

bool Application::Initialization( unsigned window_width, unsigned int window_height, const char *title ) {
    std::cout << "Program started without issue." << std::endl;

    g_pMainWindow = new Window();
    if( g_pMainWindow->Initialization( window_width, window_height, title ) != true) {
        return false;
    }

    g_pSecondaryWindow = new Window();
    if( g_pSecondaryWindow->Initialization( window_width, window_height, "Original Video" ) != true ) {
        return false;
    }
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
    std::cout << "Program finished without issue." << std::endl;
}

bool Application::ContinueProgram() {
    return AppRunning;
}