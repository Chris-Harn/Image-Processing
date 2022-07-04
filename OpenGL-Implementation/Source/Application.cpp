#include "Application.h"

#include "OpenGL\Window.h"
#include "OpenGL\Quad.h"
#include "Timer.h"
#include "Utility.h"

Window *g_pMainWindow = nullptr;
Window *g_pSecondaryWindow = nullptr;
Quad *g_pQuad = nullptr;
Timer *g_pAppTimer = nullptr;

Application::Application() : AppRunning(true) {

}

bool Application::Initialization( unsigned window_width, unsigned int window_height, float video_fps, const char *title ) {
    g_pMainWindow = new Window();
    if( g_pMainWindow->Initialization( window_width, window_height, title ) != true) {
        print_error_message( "ERROR: EXIT EARLY: Main window failed to initalize." );
        return false;
    }

    g_pSecondaryWindow = new Window();
    if( g_pSecondaryWindow->Initialization( window_width, window_height, "Original Video" ) != true ) {
        print_error_message( "ERROR: EXIT EARLY: Secondary window failed to initalize." );
        return false;
    }

    g_pQuad = new Quad();

    g_pAppTimer = new Timer();
    g_pAppTimer->Start( video_fps );

    print_message( "Program started without issue." );

    return true;
}

void Application::ProcessInput() {

}

void Application::Update() {
    //AppRunning = false;
}

void Application::Render() {
    g_pAppTimer->RestrictFrameRate();

    print_message( "Program finished one frame." );
}

void Application::CleanUp() {
    // Clean up in reverse order created
    if( g_pAppTimer != nullptr ) delete g_pAppTimer; g_pAppTimer = nullptr;
    if( g_pQuad != nullptr ) delete g_pQuad; g_pQuad = nullptr;
    if( g_pSecondaryWindow != nullptr ) delete g_pSecondaryWindow; g_pSecondaryWindow = nullptr;
    if( g_pMainWindow != nullptr ) delete g_pMainWindow; g_pMainWindow = nullptr;

    print_message( "Program finished without issue." );
}

bool Application::ContinueProgram() {
    return AppRunning;
}