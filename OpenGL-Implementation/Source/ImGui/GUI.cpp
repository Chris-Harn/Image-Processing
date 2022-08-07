#include "ImGui\GUI.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>

ImGuiViewport *g_pMainViewport;

GUI::GUI() {

}

GUI::~GUI() {
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

    // Get window size to make ImGui fullsize
    g_pMainViewport = ImGui::GetMainViewport();
}

void GUI::PollGuiEvents( ShaderControls &g_ShaderControls ) {
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

            }
            if( ImGui::MenuItem( "Save" ) ) {

            }
            if( ImGui::MenuItem( "Close" ) ) {
                g_ShaderControls.m_bcloseProgram = true;
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
    if( ImGui::CollapsingHeader( "Image Orientation" ) ) {
        ImGui::Checkbox( "Flip Horizontal Image", &g_ShaderControls.m_bflipHorizontal );
        ImGui::Checkbox( "Flip Veritical Image", &g_ShaderControls.m_bflipVertical );
    }

    if( ImGui::CollapsingHeader( "Shaders" ) ) {
        ImGui::LabelText( "Stage 1", "Input Gamma" );
        ImGui::Checkbox( "On/Off Input Gamma", &g_ShaderControls.m_binputGamma );
        ImGui::SameLine();
        if( ImGui::Button( "Reset Input Gamma" ) ) {
            g_ShaderControls.m_inputGamma = 1.0f;
        }
        ImGui::SliderFloat( "Input Gamma", &g_ShaderControls.m_inputGamma, 0.01f, 4.0f );
        ImGui::Separator();

        ImGui::LabelText( "Stage 2", "Simple Filters" );
        ImGui::Checkbox( "On/Off GaussianBlur", &g_ShaderControls.m_bguassianBlur );
        ImGui::SameLine();
        ImGui::Checkbox( "On/Off Simple Sharpen", &g_ShaderControls.m_bsharpeningPass );
        ImGui::Separator();

        ImGui::LabelText( "Stage XX", "Output Gamma" );
        ImGui::Checkbox( "On/Off Output Gamma", &g_ShaderControls.m_boutputGamma );
        ImGui::SameLine();
        if( ImGui::Button( "Reset Output Gamma" ) ) {
            g_ShaderControls.m_outputGamma = 1.0f;
        }
        ImGui::SliderFloat( "Output Gamma", &g_ShaderControls.m_outputGamma, 0.01f, 4.0f );
        ImGui::Separator();
    }

    ImGui::End();

    // Examples - Placeholder - Remove
    //ImGui::ShowDemoWindow();
}

void GUI::Draw() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
}