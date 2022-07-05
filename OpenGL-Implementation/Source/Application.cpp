#include "Application.h"

#include "OpenGL\Window.h"
#include "OpenGL\Quad.h"
#include "OpenGL\ResourceManager.h"
#include "Timer.h"
#include "Utility.h"

Window *g_pMainWindow = nullptr;
Quad *g_pQuad = nullptr;
Timer *g_pAppTimer = nullptr;

Application::Application() : AppRunning(true) {

}

bool Application::Initialization( unsigned window_width, unsigned int window_height, float video_fps, const char *title ) {
    try { g_pMainWindow = new Window(); }
    catch( const std::bad_alloc &e ) {
        (void)e;
        print_error_message( "ERROR: MEMORY ALLOCATION: Main Window failed to allocate on heap." );
        return false;
    }
    if( g_pMainWindow->Initialization( window_width, window_height, title ) != true) {
        print_error_message( "ERROR: EXIT EARLY: Main window failed to initalize." );
        return false;
    }

    try{ g_pQuad = new Quad(); }
    catch( const std::bad_alloc &e ) {
        (void)e;
        print_error_message( "ERROR: MEMORY ALLOCATION: Quad failed to allocate on heap." );
        return false;
    }

    try{ g_pAppTimer = new Timer(); }
    catch( const std::bad_alloc &e ) {
        (void)e;
        print_error_message( "ERROR: MEMORY ALLOCATION: App timer failed to allocate on heap." );
        return false;
    }
    g_pAppTimer->Start( video_fps );

    // Load Shaders, framebuffers, and other resources... 
    ResourceManager::LoadShader( "Resource/Shaders/Error/CautionStrips.glsl", "CautionImage" );

    ResourceManager::LoadShader( "Resource/Shaders/BasicBlit.glsl", "BlitImage" );
    ResourceManager::GetShader( "BlitImage" )->SetInteger( "u_Texture", 0, true );

    print_message( "Program started without issue." );

    return true;
}

void Application::ProcessInput() {

}

void Application::Update() {
    //AppRunning = false;
}

void Application::Render() {
    ResourceManager::GetShader( "CautionImage" )->Use();
    g_pQuad->RenderQuad();

    g_pMainWindow->SwapBuffers();

    g_pAppTimer->RestrictFrameRate();
}

void Application::CleanUp() {
    // Clean up in reverse order created
    if( g_pAppTimer != nullptr ) delete g_pAppTimer; g_pAppTimer = nullptr;
    if( g_pQuad != nullptr ) delete g_pQuad; g_pQuad = nullptr;
    if( g_pMainWindow != nullptr ) delete g_pMainWindow; g_pMainWindow = nullptr;

    print_message( "Program finished without issue." );
}

bool Application::ContinueProgram() {
    return AppRunning;
}