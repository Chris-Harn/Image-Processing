#include "Application.h"

#include "OpenGL\Window.h"
#include "OpenGL\Quad.h"
#include "OpenGL\ResourceManager.h"
#include "OpenGL\TextRenderer.h"
#include "OpenGL\Helper.h"
#include "ImGUI\GUI.h" 
#include "Timer.h"
#include "Utility.h"
#include "Controls.h"
#include "VideoLoader.h"
#include "VideoPlayer.h"

Application *Application::s_pInstance = 0;

ProgramControls g_ProgramControls;

Application::Application() : AppRunning(true) {
    m_pMainWindow = nullptr;
    m_pSecondaryWindow = nullptr;
    m_pGUIWindow = nullptr;
    m_pQuad = nullptr;
    m_pQuad2 = nullptr;
    m_pTextRenderer = nullptr;
    m_pGUI = nullptr;
    m_pAppTimer = nullptr;
    m_pVideoLoader = nullptr;
    m_pVideoPlayer = nullptr;
}

bool Application::Initialization( unsigned int window_width, 
                                  unsigned int window_height, 
                                  unsigned int upscale_width, 
                                  unsigned int upscale_height, 
                                  float video_fps, 
                                  const char *title ) {    
    try { m_pMainWindow = new Window(); }
    catch( const std::bad_alloc &e ) {
        (void)e;
        print_error_message( "ERROR: MEMORY ALLOCATION: Main Window failed to allocate on heap." );
        return false;
    }
    if( m_pMainWindow->Initialization( upscale_width, upscale_height, title, 0, nullptr ) != true) {
        print_error_message( "ERROR: EXIT EARLY: Main window failed to initalize." );
        return false;
    }
    try { m_pQuad = new Quad(); }
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

    ResourceManager::LoadShader( "Resource/Shaders/FlipImage.glsl", "FlipImage" );
    ResourceManager::GetShader( "FlipImage" )->SetInteger( "u_Texture", 0, true );

    ResourceManager::LoadShader( "Resource/Shaders/GammaLUT.glsl", "GammaLUT" );
    ResourceManager::GetShader( "GammaLUT" )->SetInteger( "u_Texture", 0, true );
    ResourceManager::CreateFramebuffer( window_width, window_height, "GammaInput" );
    ResourceManager::CreateFramebuffer( upscale_width, upscale_height, "GammaOutput" );

    ResourceManager::LoadShader( "Resource/Shaders/GaussianBlur5x5.glsl", "BlurImage" );
    ResourceManager::GetShader( "BlurImage" )->SetInteger( "u_Texture", 0, true );
    ResourceManager::CreateFramebuffer( window_width, window_height, "BlurOutput" );

    ResourceManager::LoadShader( "Resource/Shaders/SimpleSharpen.glsl", "SharpenImage" );
    ResourceManager::GetShader( "SharpenImage" )->SetInteger( "u_Texture", 0, true );
    ResourceManager::CreateFramebuffer( window_width, window_height, "SharpenOutput" );

    ResourceManager::LoadShader( "Resource/Shaders/FastBlitTextToScreen.glsl", "FastBlitText" );
    ResourceManager::GetShader( "FastBlitText" )->SetInteger( "text", 0, true );

    ResourceManager::LoadShader( "Resource/Shaders/ColorMedian.glsl", "ColorMedian" );
    ResourceManager::GetShader( "ColorMedian" )->SetInteger( "u_texture", 0, true );
    ResourceManager::CreateFramebuffer( window_width, window_height, "ColorMedianOutput" );

    ResourceManager::LoadShader( "Resource/Shaders/SimpleUpscale.glsl", "SimpleUpscale" );
    ResourceManager::GetShader( "SimpleUpscale" )->SetInteger( "u_texture", 0, true );
    ResourceManager::CreateFramebuffer( upscale_width, upscale_height, "SimpleUpscaleOutput" );

    ResourceManager::LoadShader( "Resource/Shaders/NearestNeighborUpscale.glsl", "NNUpscale" );
    ResourceManager::GetShader( "NNUpscale" )->SetInteger( "u_texture", 0, true );
    ResourceManager::CreateFramebuffer( upscale_width, upscale_height, "NNUpscaleOutput" );

    ResourceManager::LoadShader( "Resource/Shaders/BilinearUpscaling.glsl", "Bilinear" );
    ResourceManager::GetShader( "Bilinear" )->SetInteger( "u_texture", 0, true );
    ResourceManager::CreateFramebuffer( upscale_width, upscale_height, "BilinearOutput" );

    ResourceManager::LoadShader( "Resource/Shaders/BicubicLagrangeUpscale.glsl", "BicubicLagrange" );
    ResourceManager::GetShader( "BicubicLagrange" )->SetInteger( "u_texture", 0, true );
    ResourceManager::CreateFramebuffer( upscale_width, upscale_height, "BicubicLagrangeOutput" );

    try { m_pSecondaryWindow = new Window(); }
    catch( const std::bad_alloc &e ) {
        (void)e;
        print_error_message( "ERROR: MEMORY ALLOCATION: Secondary Window failed to allocate on heap." );
        return false;
    }
    if( m_pSecondaryWindow->Initialization( window_width, window_height, "Original Video", 1, m_pMainWindow->GetWindow() ) != true ) {
        print_error_message( "ERROR: EXIT EARLY: Secondary window failed to initalize." );
        return false;
    }

    try { m_pTextRenderer = new TextRenderer(); }
    catch( const std::bad_alloc &e ) {
        (void)e;
        print_error_message( "ERROR: MEMORY ALLOCATION: Text Renderer failed to allocate on heap." );
        return false;
    }
    m_pTextRenderer->Initialize( m_pSecondaryWindow );
    try { m_pQuad2 = new Quad(); }
    catch( const std::bad_alloc &e ) {
        (void)e;
        print_error_message( "ERROR: MEMORY ALLOCATION: Quad #2 failed to allocate on heap." );
        return false;
    }

    try { m_pGUIWindow = new Window(); }
    catch( const std::bad_alloc &e ) {
        (void)e;
        print_error_message( "ERROR: MEMORY ALLOCATION: GUI Window failed to allocate on heap." );
        return false;
    }
    if( m_pGUIWindow->Initialization( 500, 600, "Video Controls", 2, nullptr ) != true ) {
        print_error_message( "ERROR: EXIT EARLY: GUI window failed to initalize." );
        return false;
    }

    // Switch OpenGL Context back so can render correctly on both windows
    m_pMainWindow->MakeCurrentContext();

    try { m_pGUI = new GUI(); }
    catch( const std::bad_alloc &e ) {
        (void)e;
        print_error_message( "ERROR: MEMORY ALLOCATION: GUI failed to allocate on heap." );
        return false;
    }
    m_pGUI->Initialization( m_pGUIWindow->GetWindow() );

    try{ m_pAppTimer = new Timer(); }
    catch( const std::bad_alloc &e ) {
        (void)e;
        print_error_message( "ERROR: MEMORY ALLOCATION: App timer failed to allocate on heap." );
        return false;
    }
    m_pAppTimer->Tick();
    m_pAppTimer->RegulateFPS( true );

    try { m_pVideoLoader = new VideoLoader(); }
    catch( const std::bad_alloc &e ) {
        (void)e;
        print_error_message( "ERROR: MEMORY ALLOCATION: Video Loader failed to allocate on heap." );
        return false;
    }
    
    // Placeholder till can get GUI to load - Add to update function
    m_pVideoLoader->LoadNewVideo( g_ProgramControls.m_spathToCurrentVideo.c_str(), "-f image2pipe -vcodec rawvideo -pix_fmt rgb24 -" );

    try { m_pVideoPlayer = new VideoPlayer(); }
    catch( const std::bad_alloc &e ) {
        (void)e;
        print_error_message( "ERROR: MEMORY ALLOCATION: Video Player failed to allocate on heap." );
        return false;
    }

    // Placeholder till can get GUI to control
    m_pVideoPlayer->ReadyCommand(); // Tells player something is loaded
    m_pVideoPlayer->PlayCommand(); 
    bool test = m_pVideoPlayer->CurrentlyPlaying();

    print_message( "Program started without issue." );

    return true;
}

void Application::ProcessInput() {
    // Start of frame so restrict/start frame from here.
    m_pAppTimer->Tick();

    m_pMainWindow->MakeCurrentContext();
    m_pMainWindow->PollEvents();

    m_pSecondaryWindow->MakeCurrentContext();
    m_pSecondaryWindow->PollEvents();

    m_pGUIWindow->MakeCurrentContext();
    m_pGUI->PollGuiEvents( g_ProgramControls );

    // If any window should close... exit the program
    if( ( m_pMainWindow->GetShouldClose() ) ||
        ( m_pSecondaryWindow->GetShouldClose() ) ||
        ( m_pGUIWindow->GetShouldClose() ) ||
        ( g_ProgramControls.m_bcloseProgram == true ) ) AppRunning = false;
}

void Application::Update() {

}

void Application::Render() {
    /*******************************************************/
    // Render Main Window
    /*******************************************************/
    m_pMainWindow->MakeCurrentContext();
    m_pMainWindow->ClearColorBuffer();

    // Capture input frame into texture
    ResourceManager::GetFramebuffer( "OriginalVideo" )->Bind();
    if( ( m_pVideoPlayer->CurrentlyPlaying() == true ) && ( m_pVideoLoader->GrabFrameFromVideo() == true ) ) {
        // Grab frame from video and process it
        m_pVideoLoader->BindTexture( 0 );
        ResourceManager::GetShader( "FlipImage" )->Use();
        ResourceManager::GetShader( "FlipImage" )->SetBool( "u_FlipVeritical", g_ProgramControls.m_bflipVertical );
        ResourceManager::GetShader( "FlipImage" )->SetBool( "u_FlipHorizontal", g_ProgramControls.m_bflipHorizontal );
    } else {
        // Display static image on any size window
        ResourceManager::GetShader( "CautionImage" )->Use();
    }
    m_pQuad->RenderQuad();
    ResourceManager::GetFramebuffer( "OriginalVideo" )->Unbind();
    ResourceManager::GetFramebuffer( "OriginalVideo" )->BindTexture( 0 );

    m_pMainWindow->MakeCurrentContext();
    m_pMainWindow->ClearColorBuffer();

    // Update input gamma
    if( g_ProgramControls.m_binputGamma == true ) {
        ResourceManager::GetFramebuffer( "GammaInput" )->Bind();
        ResourceManager::GetShader( "GammaLUT" )->SetFloat( "u_Gamma", g_ProgramControls.m_inputGamma, true );
        m_pQuad->RenderQuad();
        ResourceManager::GetFramebuffer( "GammaInput" )->Unbind();
        ResourceManager::GetFramebuffer( "GammaInput" )->BindTexture( 0 );
    }

    // Filter - Color Median
    if( g_ProgramControls.m_bcolorMedian == true ) {
        ResourceManager::GetFramebuffer( "ColorMedianOutput" )->Bind();
        ResourceManager::GetShader( "ColorMedian" )->Use();
        m_pQuad->RenderQuad();
        ResourceManager::GetFramebuffer( "ColorMedianOutput" )->Unbind();
        ResourceManager::GetFramebuffer( "ColorMedianOutput" )->BindTexture( 0 );
    }

    // Filter - Guassian Blur 5x5
    if( g_ProgramControls.m_bguassianBlur == true ) {
        ResourceManager::GetFramebuffer( "BlurOutput" )->Bind();
        ResourceManager::GetShader( "BlurImage" )->Use();
        m_pQuad->RenderQuad();
        ResourceManager::GetFramebuffer( "BlurOutput" )->Unbind();
        ResourceManager::GetFramebuffer( "BlurOutput" )->BindTexture( 0 );
    }

    // Filter - Simple Sharpen
    if( g_ProgramControls.m_bsharpeningPass == true ) {
        ResourceManager::GetFramebuffer( "SharpenOutput" )->Bind();
        ResourceManager::GetShader( "SharpenImage" )->Use();
        m_pQuad->RenderQuad();
        ResourceManager::GetFramebuffer( "SharpenOutput" )->Unbind();
        ResourceManager::GetFramebuffer( "SharpenOutput" )->BindTexture( 0 );
    }

    // Image Upscalers
    if( g_ProgramControls.m_upscalerSelection == 0 ) {
        // Simple Interpolation
        ResourceManager::GetFramebuffer( "SimpleUpscaleOutput" )->Bind();
        ResourceManager::GetShader( "SimpleUpscale" )->Use();
        m_pQuad->RenderQuad();
        ResourceManager::GetFramebuffer( "SimpleUpscaleOutput" )->Unbind();
        ResourceManager::GetFramebuffer( "SimpleUpscaleOutput" )->BindTexture( 0 );
    } else if ( g_ProgramControls.m_upscalerSelection == 1 ) {
        // Nearest Neighbor Interpolation
        ResourceManager::GetFramebuffer( "NNUpscaleOutput" )->Bind();
        ResourceManager::GetShader( "NNUpscale" )->Use();
        m_pQuad->RenderQuad();
        ResourceManager::GetFramebuffer( "NNUpscaleOutput" )->Unbind();
        ResourceManager::GetFramebuffer( "NNUpscaleOutput" )->BindTexture( 0 );
    } else if ( g_ProgramControls.m_upscalerSelection == 2 ) {
        // Bilinear Interpolation
        ResourceManager::GetFramebuffer( "BilinearOutput" )->Bind();
        ResourceManager::GetShader( "Bilinear" )->Use();
        m_pQuad->RenderQuad();
        ResourceManager::GetFramebuffer( "BilinearOutput" )->Unbind();
        ResourceManager::GetFramebuffer( "BilinearOutput" )->BindTexture( 0 );
    } else {
        // Bicubic Lagrange Interpolation
        ResourceManager::GetFramebuffer( "BicubicLagrangeOutput" )->Bind();
        ResourceManager::GetShader( "BicubicLagrange" )->Use();
        m_pQuad->RenderQuad();
        ResourceManager::GetFramebuffer( "BicubicLagrangeOutput" )->Unbind();
        ResourceManager::GetFramebuffer( "BicubicLagrangeOutput" )->BindTexture( 0 );
    }

    // Update output gamma
    if( g_ProgramControls.m_boutputGamma == true ) {
        ResourceManager::GetFramebuffer( "GammaOutput" )->Bind();
        ResourceManager::GetShader( "GammaLUT" )->SetFloat( "u_Gamma", g_ProgramControls.m_outputGamma, true );
        m_pQuad->RenderQuad();
        ResourceManager::GetFramebuffer( "GammaOutput" )->Unbind();
        ResourceManager::GetFramebuffer( "GammaOutput" )->BindTexture( 0 );
    }

    ResourceManager::GetShader( "BlitImage" )->Use();
    m_pQuad->RenderQuad();

    m_pMainWindow->SwapBuffers();

    /*******************************************************/
    // Render Secondary Window
    /*******************************************************/
    m_pSecondaryWindow->MakeCurrentContext();
    m_pSecondaryWindow->ClearColorBuffer();

    // Blit Original Video onto second window for comparison playback
    ResourceManager::GetFramebuffer( "OriginalVideo" )->BindTexture( 0 );
    ResourceManager::GetShader( "BlitImage" )->Use();
    m_pQuad2->RenderQuad();

    if( g_ProgramControls.m_bDisplayFPS == true ) {
        DisplayPerformanceInformation();
    }

    m_pSecondaryWindow->SwapBuffers();

    /*******************************************************/
    // Render GUI Window
    /*******************************************************/
    m_pGUIWindow->MakeCurrentContext();
    m_pGUIWindow->ClearColorBuffer();
    m_pGUI->Draw();

    m_pGUIWindow->SwapBuffers();
}

void Application::CleanUp() {
    // Clean up in reverse order created
    ResourceManager::CleanUp();

    if( m_pVideoPlayer != nullptr ) delete m_pVideoPlayer; m_pVideoPlayer = nullptr;
    if( m_pVideoLoader != nullptr ) delete m_pVideoLoader; m_pVideoLoader = nullptr;
    if( m_pAppTimer != nullptr ) delete m_pAppTimer; m_pAppTimer = nullptr;
    if( m_pGUI != nullptr ) delete m_pGUI; m_pGUI = nullptr;
    if( m_pQuad2 != nullptr ) delete m_pQuad2; m_pQuad2 = nullptr;
    if( m_pQuad != nullptr ) delete m_pQuad; m_pQuad = nullptr;
    if( m_pGUIWindow != nullptr ) delete m_pGUIWindow; m_pGUIWindow = nullptr;
    if( m_pSecondaryWindow != nullptr ) delete m_pSecondaryWindow; m_pSecondaryWindow = nullptr;
    if( m_pMainWindow != nullptr ) delete m_pMainWindow; m_pMainWindow = nullptr;

    print_message( "Program finished without issue." );
}

bool Application::ContinueProgram() {
    return AppRunning;
}