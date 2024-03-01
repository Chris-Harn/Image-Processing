#include "ImGui\GUI.h"
#include "VideoPlayer.h"

#include "imgui.h"
#include "imfilebrowser.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>

ImGuiViewport *g_pMainViewport;
ImGui::FileBrowser *g_pFileDialog_avi_input;

GUI::GUI() {

}

GUI::~GUI() {
    delete g_pFileDialog_avi_input;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::Initialization( GLFWwindow *mainWindow ) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL( mainWindow, true );
    ImGui_ImplOpenGL3_Init( "#version 450 core" );

    g_pFileDialog_avi_input = new ImGui::FileBrowser();
    g_pFileDialog_avi_input->SetTitle( "Avi Input" );
    g_pFileDialog_avi_input->SetTypeFilters( { ".avi" } );

    // Get window size to make ImGui fullsize
    g_pMainViewport = ImGui::GetMainViewport();
}

void GUI::PollGuiEvents( ProgramControls &g_ProgramControls ) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Set viewport to be full size of window
    ImGui::SetNextWindowPos( ImVec2( g_pMainViewport->WorkPos.x, g_pMainViewport->WorkPos.y ), ImGuiCond_FirstUseEver );
    ImGui::SetNextWindowSize( ImVec2( g_pMainViewport->Size.x, g_pMainViewport->Size.y ), ImGuiCond_FirstUseEver );

    ImGui::Begin( "Video Controls", NULL, ImGuiWindowFlags_MenuBar | 
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | 
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | 
        ImGuiWindowFlags_NoTitleBar );
    if( ImGui::BeginMenuBar() ) {
        if( ImGui::BeginMenu( "Video" ) ) {
            if( ImGui::MenuItem( "Load" ) ) {
                g_pFileDialog_avi_input->Open();
            }
            if( ImGui::MenuItem( "Save" ) ) {

            }
            if( ImGui::MenuItem( "Close" ) ) {
                g_ProgramControls.m_bcloseProgram = true;
            }
            ImGui::EndMenu();
        }
        if( ImGui::BeginMenu( "Setting" ) ) {
            if( ImGui::MenuItem( "Load" ) ) {

            }
            if( ImGui::MenuItem( "Save" ) ) {

            }
            ImGui::EndMenu();
        }
        if( ImGui::BeginMenu( "Help" ) ) {
            if( ImGui::MenuItem( "About Program" ) ) {

            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    if( ImGui::CollapsingHeader( "Video Controls" ) ) {
        if( ImGui::SmallButton( "Play" ) ) TheVideoPlayer::Instance()->PlayCommand();
        ImGui::SameLine();
        if( ImGui::SmallButton( "Pause" ) ) TheVideoPlayer::Instance()->PauseCommand();  
    }

    if( ImGui::CollapsingHeader( "Image Orientation" ) ) {
        ImGui::Checkbox( "Display FPS", &g_ProgramControls.m_bDisplayFPS );
        ImGui::Checkbox( "Flip Horizontal Image", &g_ProgramControls.m_bflipHorizontal );
        ImGui::Checkbox( "Flip Veritical Image", &g_ProgramControls.m_bflipVertical );
    }

    if( ImGui::CollapsingHeader( "Shaders" ) ) {
        ImGui::LabelText( "Stage 1", "Input Gamma" );
        ImGui::Checkbox( "On/Off Input Gamma", &g_ProgramControls.m_binputGamma );
        ImGui::SameLine();
        if( ImGui::Button( "Reset Input Gamma" ) ) {
            g_ProgramControls.m_inputGamma = 1.0f;
        }
        ImGui::SliderFloat( "Input Gamma", &g_ProgramControls.m_inputGamma, 0.01f, 4.0f );
        ImGui::Separator();

        ImGui::LabelText( "Stage 2", "Simple Filters" );
        ImGui::Checkbox( "On/Off Color Median", &g_ProgramControls.m_bcolorMedian );
        ImGui::Separator();
        ImGui::Checkbox( "On/Off GaussianBlur", &g_ProgramControls.m_bguassianBlur );
        ImGui::SameLine();
        ImGui::Checkbox( "On/Off Simple Sharpen", &g_ProgramControls.m_bsharpeningPass );
        ImGui::Separator();
        ImGui::Checkbox( "On/Off Enhanced Saturation", &g_ProgramControls.m_bEnhanceSaturation );
        ImGui::SameLine();
        if( ImGui::Button( "Reset Saturation" ) ) {
            g_ProgramControls.m_enhanceSaturation = 1.0f;
        }
        ImGui::SliderFloat( "Saturation", &g_ProgramControls.m_enhanceSaturation, 0.01f, 1.0f );
        ImGui::Separator();

        ImGui::LabelText( "Stage 3", "Histogram Manipulation" );
        ImGui::Checkbox( "On/Off Color Histogram Spread", &g_ProgramControls.m_bHistoramSpread );
        if( g_ProgramControls.m_bHistoramSpread == true ) {
            ImGui::Text( "Original Luminance" );
            ImGui::PushItemWidth( -1 );
            ImGui::PlotHistogram( "", g_ProgramControls.m_histogram, 512,
                0, nullptr, 0.0f, float( 3000 ), ImVec2( 0, 80 ) );
            ImGui::PopItemWidth();
            ImGui::Text( "Updated Luminance" );
            ImGui::PushItemWidth( -1 );
            ImGui::PlotHistogram( "", g_ProgramControls.m_backProjection, 512,
                0, nullptr, 0.0f, float( 3000 ), ImVec2( 0, 80 ) );
            ImGui::PopItemWidth();     
        }



        ImGui::Separator();
  
        ImGui::LabelText( "Stage 4", "Image Upscalers" );
        ImGui::RadioButton( "Simple Upscaler", &g_ProgramControls.m_upscalerSelection, 0 ); ImGui::SameLine();
        ImGui::RadioButton( "Nearest Neighbor", &g_ProgramControls.m_upscalerSelection, 1 ); ImGui::SameLine();
        ImGui::RadioButton( "Bilinear Inter.", &g_ProgramControls.m_upscalerSelection, 2 );
        ImGui::RadioButton( "Bicubic Lagrange Inter.", &g_ProgramControls.m_upscalerSelection, 3 );
        ImGui::Separator();

        ImGui::LabelText( "Stage 5", "Final Filters" );
        ImGui::Checkbox( "On/Off Anti-Aliasing", &g_ProgramControls.m_bAntiAliasing );
        ImGui::Separator();

        ImGui::LabelText( "Stage 6", "Output Gamma" );
        ImGui::Checkbox( "On/Off Output Gamma", &g_ProgramControls.m_boutputGamma );
        ImGui::SameLine();
        if( ImGui::Button( "Reset Output Gamma" ) ) {
            g_ProgramControls.m_outputGamma = 1.0f;
        }
        ImGui::SliderFloat( "Output Gamma", &g_ProgramControls.m_outputGamma, 0.01f, 4.0f );
        ImGui::Separator();
    }

    ImGui::End();

    // Handle File System
    g_pFileDialog_avi_input->Display();
    if( g_pFileDialog_avi_input->HasSelected() ) {
        std::cout << "Selected filename" << g_pFileDialog_avi_input->GetSelected().string() << std::endl;
        std::size_t length = g_pFileDialog_avi_input->GetSelected().string().length();
        g_ProgramControls.m_spathToCurrentVideo.replace( 0, length, (char *)g_pFileDialog_avi_input->GetSelected().string().c_str() );
        std::cout << "Correctly updated program to... " << g_ProgramControls.m_spathToCurrentVideo << std::endl << std::endl;

        g_pFileDialog_avi_input->ClearSelected();
    }

    // Examples - Placeholder - Remove
    //ImGui::ShowDemoWindow();
}

void GUI::Draw() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
}