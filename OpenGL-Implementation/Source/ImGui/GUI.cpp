#include "ImGui\GUI.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>

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
}

void GUI::PollGuiEvents( ShaderControls &g_ShaderControls ) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Set viewport to be full size of window
    const ImGuiViewport *main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos( ImVec2( main_viewport->WorkPos.x, main_viewport->WorkPos.y ), ImGuiCond_FirstUseEver );
    ImGui::SetNextWindowSize( ImVec2( main_viewport->Size.x, main_viewport->Size.y ), ImGuiCond_FirstUseEver );

    ImGui::Begin( "Image Processing.", NULL, ImGuiWindowFlags_MenuBar | 
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | 
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | 
        ImGuiWindowFlags_NoTitleBar );
    if( ImGui::BeginMenuBar() ) {
        if( ImGui::BeginMenu( "File" ) ) {
            if( ImGui::MenuItem( "Load File" ) ) {

            }
            if( ImGui::MenuItem( "Save File" ) ) {

            }
            if( ImGui::MenuItem( "Close Program" ) ) {

            }
            ImGui::EndMenu();
        }
        if( ImGui::BeginMenu( "Setting" ) ) {
            ImGui::EndMenu();
        }
        if( ImGui::BeginMenu( "Help" ) ) {
            if( ImGui::MenuItem( "About Program" ) ) {

            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::LabelText( "Stage 1", "Input Gamma" );
    ImGui::Checkbox( "On/Off Input Gamma", &g_ShaderControls.m_binputGamma );
    ImGui::SameLine();
    if( ImGui::Button( "Reset Input Gamma" ) ) {
        g_ShaderControls.m_inputGamma = 1.0f;
    }
    ImGui::SliderFloat( "Input Gamma", &g_ShaderControls.m_inputGamma, 0.01f, 4.0f );
    ImGui::Separator();

    ImGui::LabelText( "Stage X", "Output Gamma" );
    ImGui::Checkbox( "On/Off Output Gamma", &g_ShaderControls.m_boutputGamma );
    ImGui::SameLine();
    if( ImGui::Button( "Reset Output Gamma" ) ) {
        g_ShaderControls.m_outputGamma = 1.0f;
    }
    ImGui::SliderFloat( "Output Gamma", &g_ShaderControls.m_outputGamma, 0.01f, 4.0f );
    ImGui::Separator();
    
    ImGui::End();
}

void GUI::Draw() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
}