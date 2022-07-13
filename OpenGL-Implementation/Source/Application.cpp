#include "Application.h"

#include "OpenGL\Window.h"
#include "OpenGL\Quad.h"
#include "OpenGL\ResourceManager.h"
#include "ImGUI\GUI.h" 
#include "Timer.h"
#include "Utility.h"
#include "Controls.h"
#include "VideoLoader.h"

Window *g_pMainWindow = nullptr;
Window *g_pSecondaryWindow = nullptr;
Window *g_pGUIWindow = nullptr;
Quad *g_pQuad = nullptr;
Quad *g_pQuad2 = nullptr;
GUI *g_pGUI = nullptr;
Timer *g_pAppTimer = nullptr;
VideoLoader *g_pVideoLoader = nullptr;
ShaderControls g_ShaderControls;

Application::Application() : AppRunning(true) {

}

bool Application::Initialization( unsigned int window_width, unsigned int window_height, float video_fps, const char *title ) {    
    try { g_pMainWindow = new Window(); }
    catch( const std::bad_alloc &e ) {
        (void)e;
        print_error_message( "ERROR: MEMORY ALLOCATION: Main Window failed to allocate on heap." );
        return false;
    }
    if( g_pMainWindow->Initialization( window_width, window_height, title, 0, nullptr ) != true) {
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
    // Note: FBO's and VBO's are not shared between OpenGL contexts
    ResourceManager::LoadShader( "Resource/Shaders/Error/CautionStrips.glsl", "CautionImage" );
    ResourceManager::CreateFramebuffer( window_width, window_height, "OriginalVideo" );

    ResourceManager::LoadShader( "Resource/Shaders/BasicBlit.glsl", "BlitImage" );
    ResourceManager::GetShader( "BlitImage" )->SetInteger( "u_Texture", 0, true );
    ResourceManager::CreateFramebuffer( window_width, window_height, "InputImage" );

    ResourceManager::LoadShader( "Resource/Shaders/GammaLUT.glsl", "GammaLUT" );
    ResourceManager::GetShader( "GammaLUT" )->SetInteger( "u_Texture", 0, true );
    ResourceManager::CreateFramebuffer( window_width, window_height, "GammaInput" );
    ResourceManager::CreateFramebuffer( window_width, window_height, "GammaOutput" );

    try { g_pSecondaryWindow = new Window(); }
    catch( const std::bad_alloc &e ) {
        (void)e;
        print_error_message( "ERROR: MEMORY ALLOCATION: Secondary Window failed to allocate on heap." );
        return false;
    }
    if( g_pSecondaryWindow->Initialization( window_width, window_height, "Original Video", 1, g_pMainWindow->GetWindow() ) != true ) {
        print_error_message( "ERROR: EXIT EARLY: Secondary window failed to initalize." );
        return false;
    }
    try { g_pQuad2 = new Quad(); }
    catch( const std::bad_alloc &e ) {
        (void)e;
        print_error_message( "ERROR: MEMORY ALLOCATION: Quad #2 failed to allocate on heap." );
        return false;
    }

    try { g_pGUIWindow = new Window(); }
    catch( const std::bad_alloc &e ) {
        (void)e;
        print_error_message( "ERROR: MEMORY ALLOCATION: GUI Window failed to allocate on heap." );
        return false;
    }
    if( g_pGUIWindow->Initialization( 500, 600, "Video Controls", 2, nullptr ) != true ) {
        print_error_message( "ERROR: EXIT EARLY: GUI window failed to initalize." );
        return false;
    }

    // Switch OpenGL Context back so can render correctly on both windows
    g_pMainWindow->MakeCurrentContext();

    try { g_pGUI = new GUI(); }
    catch( const std::bad_alloc &e ) {
        (void)e;
        print_error_message( "ERROR: MEMORY ALLOCATION: GUI failed to allocate on heap." );
        return false;
    }
    g_pGUI->Initialization( g_pGUIWindow->GetWindow() );

    try{ g_pAppTimer = new Timer(); }
    catch( const std::bad_alloc &e ) {
        (void)e;
        print_error_message( "ERROR: MEMORY ALLOCATION: App timer failed to allocate on heap." );
        return false;
    }
    g_pAppTimer->Start( video_fps );

    try { g_pVideoLoader = new VideoLoader(); }
    catch( const std::bad_alloc &e ) {
        (void)e;
        print_error_message( "ERROR: MEMORY ALLOCATION: Video Loader failed to allocate on heap." );
        return false;
    }
    
    // Placeholder till can get GUI to load
    g_pVideoLoader->LoadNewVideo( "../../VHS_1980.avi", "-f image2pipe -vcodec rawvideo -pix_fmt rgb24 -" );

    // Setp controls with default values
    g_ShaderControls.m_inputGamma = 1.0f;
    g_ShaderControls.m_outputGamma = 1.0f;

    print_message( "Program started without issue." );

    return true;
}

void Application::ProcessInput() {
    g_pMainWindow->MakeCurrentContext();
    g_pMainWindow->PollEvents();

    g_pSecondaryWindow->MakeCurrentContext();
    g_pSecondaryWindow->PollEvents();

    g_pGUIWindow->MakeCurrentContext();
    g_pGUI->PollGuiEvents( g_ShaderControls );

    // If any window should close... exit the program
    if( ( g_pMainWindow->GetShouldClose() ) ||
        ( g_pSecondaryWindow->GetShouldClose() ) ||
        ( g_pGUIWindow->GetShouldClose() ) ||
        ( g_ShaderControls.m_bcloseProgram == true ) ) AppRunning = false;
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

    // Update input gamma
    if( g_ShaderControls.m_binputGamma == true ) {
        ResourceManager::GetFramebuffer( "GammaInput" )->Bind();
        ResourceManager::GetShader( "GammaLUT" )->SetFloat( "u_Gamma", g_ShaderControls.m_inputGamma, true );
        g_pQuad->RenderQuad();
        ResourceManager::GetFramebuffer( "GammaInput" )->Unbind();
        ResourceManager::GetFramebuffer( "GammaInput" )->BindTexture( 0 );
    }

    // Update output gamma
    if( g_ShaderControls.m_boutputGamma == true ) {
        ResourceManager::GetFramebuffer( "GammaOutput" )->Bind();
        ResourceManager::GetShader( "GammaLUT" )->SetFloat( "u_Gamma", g_ShaderControls.m_outputGamma, true );
        g_pQuad->RenderQuad();
        ResourceManager::GetFramebuffer( "GammaOutput" )->Unbind();
        ResourceManager::GetFramebuffer( "GammaOutput" )->BindTexture( 0 );
    }

    ResourceManager::GetShader( "BlitImage" )->Use();
    g_pQuad->RenderQuad();

    g_pMainWindow->SwapBuffers();

    /*******************************************************/
    // Render Secondary Window
    /*******************************************************/
    g_pSecondaryWindow->MakeCurrentContext();
    g_pSecondaryWindow->ClearColorBuffer();

    // Blit Original Video onto second window for comparison playback
    ResourceManager::GetFramebuffer( "OriginalVideo" )->BindTexture( 0 );
    ResourceManager::GetShader( "BlitImage" )->Use();
    g_pQuad2->RenderQuad();

    g_pSecondaryWindow->SwapBuffers();

    /*******************************************************/
    // Render GUI Window
    /*******************************************************/
    g_pGUIWindow->MakeCurrentContext();
    g_pGUIWindow->ClearColorBuffer();
    g_pGUI->Draw();

    g_pGUIWindow->SwapBuffers();

    /*******************************************************/
    // Final Actions for Loop
    /*******************************************************/
    g_pAppTimer->RestrictFrameRate();
}

void Application::CleanUp() {
    // Clean up in reverse order created
    ResourceManager::CleanUp();
    if( g_pAppTimer != nullptr ) delete g_pAppTimer; g_pAppTimer = nullptr;
    if( g_pGUI != nullptr ) delete g_pGUI; g_pGUI = nullptr;
    if( g_pQuad2 != nullptr ) delete g_pQuad2; g_pQuad2 = nullptr;
    if( g_pQuad != nullptr ) delete g_pQuad; g_pQuad = nullptr;
    if( g_pSecondaryWindow != nullptr ) delete g_pSecondaryWindow; g_pSecondaryWindow = nullptr;
    if( g_pMainWindow != nullptr ) delete g_pMainWindow; g_pMainWindow = nullptr;

    print_message( "Program finished without issue." );
}

bool Application::ContinueProgram() {
    return AppRunning;
}