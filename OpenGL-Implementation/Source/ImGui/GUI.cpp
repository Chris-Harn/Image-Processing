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

void GUI::StartFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GUI::DrawGui( ShaderControls &g_ShaderControls ) {
    ImGui::Begin( "GUI is created.", NULL );
    ImGui::SliderFloat( "Input Gamma", &g_ShaderControls.m_inputGamma, 0.01f, 4.0f );
    ImGui::SliderFloat( "Output Gamma", &g_ShaderControls.m_outputGamma, 0.01f, 4.0f );
    ImGui::End();
}

void GUI::Draw() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
}