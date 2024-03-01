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

#include <iostream>

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
    m_pMainWindow->GetVersion();

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

    ResourceManager::LoadShader( "Resource/Shaders/EnhancedSaturation.glsl", "EnhancedSaturation" );
    ResourceManager::GetShader( "EnhancedSaturation" )->SetInteger( "u_Texture", 0, true );
    ResourceManager::CreateFramebuffer( window_width, window_height, "EnhancedSaturationOutput" );

    ResourceManager::LoadShader( "Resource/Shaders/CollectHistogram.glsl", "CollectHistogram" );
    ResourceManager::GetShader( "CollectHistogram" )->SetInteger( "u_Texture", 0, true );
    ResourceManager::CreateFramebuffer( 512, 1, "CollectHistogramOutput", true );

    ResourceManager::LoadShader( "Resource/Shaders/BackProjectionUpdate.glsl", "BackProjection" );
    ResourceManager::GetShader( "BackProjection" )->SetInteger( "u_Texture", 0, true );
    ResourceManager::GetShader( "BackProjection" )->SetInteger( "u_BackProjection", 1, true );
    ResourceManager::CreateFramebuffer( window_width, window_height, "BackProjectionOutput" );

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

    ResourceManager::LoadShader( "Resource/Shaders/AntiAliasing.glsl", "AntiAliasing" );
    ResourceManager::GetShader( "AntiAliasing" )->SetInteger( "u_texture", 0, true );
    ResourceManager::CreateFramebuffer( upscale_width, upscale_height, "AntiAliasingOutput" );


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

    // Hardcoded video path, but can setup the Video Player for now
    TheVideoPlayer::Instance()->ReadyCommand();
    TheVideoPlayer::Instance()->PlayCommand();

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
    // NOTE: m_pVideoLoader->GrabFrameFromVideo() does not load up the next frame if the VCR
    // controls are not set to play. Only in play will it load the next video frame.
    if( ( TheVideoPlayer::Instance()->CurrentlyPlaying() == true ) && ( m_pVideoLoader->GrabFrameFromVideo() == true ) ) {
        // Grab frame from video and process it
        m_pVideoLoader->BindTexture( 0 );
        ResourceManager::GetShader( "FlipImage" )->Use();
        ResourceManager::GetShader( "FlipImage" )->SetBool( "u_FlipVeritical", g_ProgramControls.m_bflipVertical );
        ResourceManager::GetShader( "FlipImage" )->SetBool( "u_FlipHorizontal", g_ProgramControls.m_bflipHorizontal );
    } else if ( TheVideoPlayer::Instance()->CurrentlyPaused() == true ) {
        // Paused, but allow image filter changes on last image loaded.
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
    
    // Save so can jump straight to last framebuffer
    ResourceManager::SaveLastFramebuffer( "OriginalVideo" );

    // Update input gamma
    if( g_ProgramControls.m_binputGamma == true ) {
        ResourceManager::GetFramebuffer( "GammaInput" )->Bind();
        ResourceManager::GetShader( "GammaLUT" )->SetFloat( "u_Gamma", g_ProgramControls.m_inputGamma, true );
        m_pQuad->RenderQuad();
        ResourceManager::GetFramebuffer( "GammaInput" )->Unbind();
        ResourceManager::GetFramebuffer( "GammaInput" )->BindTexture( 0 );

        // Save so can jump straight to last framebuffer
        ResourceManager::SaveLastFramebuffer( "GammaInput" );
    }

    // Filter - Color Median
    if( g_ProgramControls.m_bcolorMedian == true ) {
        ResourceManager::GetFramebuffer( "ColorMedianOutput" )->Bind();
        ResourceManager::GetShader( "ColorMedian" )->Use();
        m_pQuad->RenderQuad();
        ResourceManager::GetFramebuffer( "ColorMedianOutput" )->Unbind();
        ResourceManager::GetFramebuffer( "ColorMedianOutput" )->BindTexture( 0 );

        // Save so can jump straight to last framebuffer
        ResourceManager::SaveLastFramebuffer( "ColorMedianOutput" );
    }

    // Filter - Guassian Blur 5x5
    if( g_ProgramControls.m_bguassianBlur == true ) {
        ResourceManager::GetFramebuffer( "BlurOutput" )->Bind();
        ResourceManager::GetShader( "BlurImage" )->Use();
        m_pQuad->RenderQuad();
        ResourceManager::GetFramebuffer( "BlurOutput" )->Unbind();
        ResourceManager::GetFramebuffer( "BlurOutput" )->BindTexture( 0 );

        // Save so can jump straight to last framebuffer
        ResourceManager::SaveLastFramebuffer( "BlurOutput" );
    }

    // Filter - Simple Sharpen
    if( g_ProgramControls.m_bsharpeningPass == true ) {
        ResourceManager::GetFramebuffer( "SharpenOutput" )->Bind();
        ResourceManager::GetShader( "SharpenImage" )->Use();
        m_pQuad->RenderQuad();
        ResourceManager::GetFramebuffer( "SharpenOutput" )->Unbind();
        ResourceManager::GetFramebuffer( "SharpenOutput" )->BindTexture( 0 );

        // Save so can jump straight to last framebuffer
        ResourceManager::SaveLastFramebuffer( "SharpenOutput" );
    }

    // Filter - Enhanced Saturation
    if( g_ProgramControls.m_bEnhanceSaturation == true ) {
        ResourceManager::GetFramebuffer( "EnhancedSaturationOutput" )->Bind();
        ResourceManager::GetShader( "EnhancedSaturation" )->Use();
        ResourceManager::GetShader( "EnhancedSaturation" )->SetFloat( "u_fract", g_ProgramControls.m_enhanceSaturation, true );
        m_pQuad->RenderQuad();
        ResourceManager::GetFramebuffer( "EnhancedSaturationOutput" )->Unbind();
        ResourceManager::GetFramebuffer( "EnhancedSaturationOutput" )->BindTexture( 0 );

        // Save so can jump straight to last framebuffer
        ResourceManager::SaveLastFramebuffer( "EnhancedSaturationOutput" );
    }

    // Filter - Histogram Equalization
    if( g_ProgramControls.m_bHistoramSpread == true ) {
        // Setup render state
        glEnable( GL_BLEND );
        glDisable( GL_POINT_SMOOTH );
        glBlendFunc( GL_ONE, GL_ONE );
        glBlendEquation(GL_FUNC_ADD);
        glEnable( GL_PROGRAM_POINT_SIZE );
        glDepthMask( GL_FALSE );

        // zero out histogram
        for( int i = 0; i < 512; i++ ) {
            g_ProgramControls.m_histogram[i] = 0.0f;
            g_ProgramControls.m_stretchHistogram[i] = 0.0f;
            g_ProgramControls.m_backProjection[i] = 0.0f;
        }

        // Step 1 - Collect histogram from all three colors 
        // and ReadPixels to CPU of three histograms
        // Red
        glViewport( 0, 0, 512, 1 );
        ResourceManager::GetFramebuffer( "CollectHistogramOutput" )->Bind();
        ResourceManager::GetShader( "CollectHistogram" )->Use();
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glDrawArrays( GL_POINTS, 0, 640 * 480 );
        glReadPixels( 0, 0, 512, 1, GL_RED, GL_FLOAT, g_ProgramControls.m_histogram );

        ResourceManager::GetFramebuffer( "CollectHistogramOutput" )->Unbind();
        ResourceManager::GetFramebuffer( "CollectHistogramOutput" )->BindTexture( 0 );

        // Set back statemachine that would affect other filters
        glDisable( GL_BLEND );
        glViewport( 0, 0, 640, 480 );

        // Step 2 - Create backprojection maps based on inputs
        // Cummative Sum histogram to create backprojection map
        float sum = 0.0f;
        float scaleFactor = 511.0f / ( 640 * 480 );
        //std::cout << "Printing backprojection... " << std::endl;
        for( int i = 0; i < 512; i++ ) {
            sum += g_ProgramControls.m_histogram[i];
            g_ProgramControls.m_backProjection[i] = ( ( sum * scaleFactor ) + 0.5f ) / 512.0f;

            //if( i < 10 || i > 501 ) {
            //    std::cout << i << " = " << g_ProgramControls.m_histogram[i] << "    back projection = " << g_ProgramControls.m_backProjection[i] << "    sum = " << sum << std::endl;
            //} 
        }
        //std::cout << std::endl << std::endl;

        // Step 3 - Send reprojection maps to the GPU
        ResourceManager::GetFramebuffer( "CollectHistogramOutput" )->BindTexture( 0 );
        glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 512, 1, GL_RED, GL_FLOAT, g_ProgramControls.m_backProjection );
        //glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 512, 1, GL_RED, GL_FLOAT, g_ProgramControls.m_stretchHistogram );

        // Step 4 - Update image based on three reprojection maps
        ResourceManager::GetFramebuffer( "BackProjectionOutput" )->Bind();
        ResourceManager::GetShader( "BackProjection" )->Use();
        ResourceManager::GetLastFramebuffer()->BindTexture( 0 ); // Last image filter output
        ResourceManager::GetFramebuffer( "CollectHistogramOutput" )->BindTexture( 1 ); // Final histogram
        m_pQuad->RenderQuad();
        ResourceManager::GetFramebuffer( "BackProjectionOutput" )->Unbind();
        ResourceManager::GetFramebuffer( "BackProjectionOutput" )->BindTexture( 0 );
        ResourceManager::SaveLastFramebuffer( "BackProjectionOutput" );

        // Step 5 - Get new luminance histogram out to compare
        // Setup render state
        glEnable( GL_BLEND );
        glDisable( GL_POINT_SMOOTH );
        glBlendFunc( GL_ONE, GL_ONE );
        glBlendEquation( GL_FUNC_ADD );
        glEnable( GL_PROGRAM_POINT_SIZE );
        glDepthMask( GL_FALSE );

        // zero out histogram
        for( int i = 0; i < 512; i++ ) {
            g_ProgramControls.m_backProjection[i] = 0.0f;
        }

        glViewport( 0, 0, 512, 1 );
        ResourceManager::GetFramebuffer( "CollectHistogramOutput" )->Bind();
        ResourceManager::GetShader( "CollectHistogram" )->Use();
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glDrawArrays( GL_POINTS, 0, 640 * 480 );
        glReadPixels( 0, 0, 512, 1, GL_RED, GL_FLOAT, g_ProgramControls.m_backProjection );

        ResourceManager::GetFramebuffer( "CollectHistogramOutput" )->Unbind();

        // Set back statemachine that would affect other filters
        glDisable( GL_BLEND );
        glViewport( 0, 0, 640, 480 );
    }

    // Image Upscalers
    if( g_ProgramControls.m_upscalerSelection == 0 ) {
        // Simple Interpolation
        ResourceManager::GetFramebuffer( "SimpleUpscaleOutput" )->Bind();
        ResourceManager::GetShader( "SimpleUpscale" )->Use();
        ResourceManager::GetLastFramebuffer()->BindTexture( 0 );
        m_pQuad->RenderQuad();
        ResourceManager::GetFramebuffer( "SimpleUpscaleOutput" )->Unbind();
        ResourceManager::GetFramebuffer( "SimpleUpscaleOutput" )->BindTexture( 0 );
    } else if ( g_ProgramControls.m_upscalerSelection == 1 ) {
        // Nearest Neighbor Interpolation
        ResourceManager::GetFramebuffer( "NNUpscaleOutput" )->Bind();
        ResourceManager::GetShader( "NNUpscale" )->Use();
        ResourceManager::GetLastFramebuffer()->BindTexture( 0 );
        m_pQuad->RenderQuad();
        ResourceManager::GetFramebuffer( "NNUpscaleOutput" )->Unbind();
        ResourceManager::GetFramebuffer( "NNUpscaleOutput" )->BindTexture( 0 );
    } else if ( g_ProgramControls.m_upscalerSelection == 2 ) {
        // Bilinear Interpolation
        ResourceManager::GetFramebuffer( "BilinearOutput" )->Bind();
        ResourceManager::GetShader( "Bilinear" )->Use();
        ResourceManager::GetLastFramebuffer()->BindTexture( 0 );
        m_pQuad->RenderQuad();
        ResourceManager::GetFramebuffer( "BilinearOutput" )->Unbind();
        ResourceManager::GetFramebuffer( "BilinearOutput" )->BindTexture( 0 );
    } else {
        // Bicubic Lagrange Interpolation
        ResourceManager::GetFramebuffer( "BicubicLagrangeOutput" )->Bind();
        ResourceManager::GetShader( "BicubicLagrange" )->Use();
        ResourceManager::GetLastFramebuffer()->BindTexture( 0 );
        m_pQuad->RenderQuad();
        ResourceManager::GetFramebuffer( "BicubicLagrangeOutput" )->Unbind();
        ResourceManager::GetFramebuffer( "BicubicLagrangeOutput" )->BindTexture( 0 );
    }

    // Anti-Aliasing Filter
    if( g_ProgramControls.m_bAntiAliasing == true ) {
        ResourceManager::GetFramebuffer( "AntiAliasingOutput" )->Bind();
        ResourceManager::GetShader( "AntiAliasing" )->Use();
        ResourceManager::GetLastFramebuffer()->BindTexture( 0 );
        m_pQuad->RenderQuad();
        ResourceManager::GetFramebuffer( "AntiAliasingOutput" )->Unbind();
        ResourceManager::GetFramebuffer( "AntiAliasingOutput" )->BindTexture( 0 );
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