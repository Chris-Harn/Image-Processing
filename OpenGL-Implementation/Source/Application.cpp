#include "Application.h"

#include "OpenGL\Window.h"
#include "OpenGL\Quad.h"
#include "OpenGL\ResourceManager.h"
#include "Gui.h"
#include "Timer.h"
#include "Utility.h"

Window *g_pMainWindow = nullptr;
Window *g_pSecondaryWindow = nullptr;
Quad *g_pQuad = nullptr;
Quad *g_pQuad2 = nullptr;
Timer *g_pAppTimer = nullptr;

Application::Application() : AppRunning(true) {

}

bool Application::Initialization( unsigned int window_width, unsigned int window_height, float video_fps, const char *title ) {    
    try { g_pMainWindow = new Window(); }
    catch( const std::bad_alloc &e ) {
        (void)e;
        print_error_message( "ERROR: MEMORY ALLOCATION: Main Window failed to allocate on heap." );
        return false;
    }
    if( g_pMainWindow->Initialization( window_width, window_height, title, false ) != true) {
        print_error_message( "ERROR: EXIT EARLY: Main window failed to initalize." );
        return false;
    }
    try { g_pQuad = new Quad(); }
    catch( const std::bad_alloc &e ) {
        (void)e;
        print_error_message( "ERROR: MEMORY ALLOCATION: Quad #1 failed to allocate on heap." );
        return false;
    }

    // Load Shaders, framebuffers, and other resources... for OpenGL Context #1
    ResourceManager::LoadShader( "Resource/Shaders/Error/CautionStrips.glsl", "CautionImage" );
    ResourceManager::CreateFramebuffer( window_width, window_height, "OriginalVideo" );

    ResourceManager::LoadShader( "Resource/Shaders/BasicBlit.glsl", "BlitImage" );
    ResourceManager::GetShader( "BlitImage" )->SetInteger( "u_Texture", 0, true );

    try { g_pSecondaryWindow = new Window(); }
    catch( const std::bad_alloc &e ) {
        (void)e;
        print_error_message( "ERROR: MEMORY ALLOCATION: Secondary Window failed to allocate on heap." );
        return false;
    }
    if( g_pSecondaryWindow->Initialization( window_width, window_height, "Original Video", true ) != true ) {
        print_error_message( "ERROR: EXIT EARLY: Secondary window failed to initalize." );
        return false;
    }
    try { g_pQuad2 = new Quad(); }
    catch( const std::bad_alloc &e ) {
        (void)e;
        print_error_message( "ERROR: MEMORY ALLOCATION: Quad #2 failed to allocate on heap." );
        return false;
    }

    // Load Shaders, framebuffers, and other resources... for OpenGL Context #2
    // Note: Seperate OpenGL Contexts each need their own shaders
    ResourceManager::LoadShader( "Resource/Shaders/Error/CautionStrips.glsl", "CautionImageTest" );
    ResourceManager::CreateFramebuffer( window_width, window_height, "OriginalVideoPlayback" );

    ResourceManager::LoadShader( "Resource/Shaders/BasicBlit.glsl", "BlitImagePlayback" );
    ResourceManager::GetShader( "BlitImagePlayback" )->SetInteger( "u_Texture", 0, true );

    // Switch OpenGL Context back so can render correctly on both windows
    g_pMainWindow->MakeCurrentContext();

    try{ g_pAppTimer = new Timer(); }
    catch( const std::bad_alloc &e ) {
        (void)e;
        print_error_message( "ERROR: MEMORY ALLOCATION: App timer failed to allocate on heap." );
        return false;
    }
    g_pAppTimer->Start( video_fps );

    print_message( "Program started without issue." );

    return true;
}

void Application::ProcessInput() {
    g_pMainWindow->PollEvents();

    // If Window #1 or Window #2 close... exit the program
    if( ( g_pMainWindow->GetShouldClose() ) ||
        ( g_pSecondaryWindow->GetShouldClose() ) ) AppRunning = false;
}

void Application::Update() {

}

void Application::Render() {
    /*******************************************************/
    // Render Main Window
    /*******************************************************/
    g_pMainWindow->MakeCurrentContext();
    g_pMainWindow->ClearColorBuffer();

    // Capture input frame into texture
    ResourceManager::GetFramebuffer( "OriginalVideo" )->Bind();
    ResourceManager::GetShader( "CautionImage" )->Use();
    g_pQuad->RenderQuad();
    ResourceManager::GetFramebuffer( "OriginalVideo" )->Unbind();
    ResourceManager::GetFramebuffer( "OriginalVideo" )->BindTexture( 0 );

    // Blit that image onto its window
    ResourceManager::GetShader( "BlitImage" )->SetInteger( "u_Texture", 0, true );
    g_pQuad->RenderQuad();

    g_pMainWindow->SwapBuffers();


    /*******************************************************/
    // Render Secondary Window
    /*******************************************************/
    g_pSecondaryWindow->MakeCurrentContext();
    g_pSecondaryWindow->ClearColorBuffer();

    // Capture input frame into texture
    ResourceManager::GetFramebuffer( "OriginalVideoPlayback" )->Bind();
    ResourceManager::GetShader( "CautionImageTest" )->Use();
    g_pQuad2->RenderQuad();
    ResourceManager::GetFramebuffer( "OriginalVideoPlayback" )->Unbind();
    ResourceManager::GetFramebuffer( "OriginalVideoPlayback" )->BindTexture( 0 );

    // Blit Original Video onto second window
    ResourceManager::GetShader( "BlitImagePlayback" )->Use();
    g_pQuad2->RenderQuad();

    g_pSecondaryWindow->SwapBuffers();

    /*******************************************************/
    // Final Actions for Loop
    /*******************************************************/
    g_pAppTimer->RestrictFrameRate();
}

void Application::CleanUp() {
    // Clean up in reverse order created
    if( g_pAppTimer != nullptr ) delete g_pAppTimer; g_pAppTimer = nullptr;
    if( g_pQuad2 != nullptr ) delete g_pQuad2; g_pQuad2 = nullptr;
    if( g_pQuad != nullptr ) delete g_pQuad; g_pQuad = nullptr;
    if( g_pSecondaryWindow != nullptr ) delete g_pSecondaryWindow; g_pSecondaryWindow = nullptr;
    if( g_pMainWindow != nullptr ) delete g_pMainWindow; g_pMainWindow = nullptr;

    // Get rid of all shaders and textures
    ResourceManager::CleanUp();

    print_message( "Program finished without issue." );
}

bool Application::ContinueProgram() {
    return AppRunning;
}