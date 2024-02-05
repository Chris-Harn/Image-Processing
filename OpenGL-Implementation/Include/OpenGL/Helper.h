#include "OpenGL\TextRenderer.h"

extern TextRenderer *g_pTextRenderer;

#include <string>

void DisplayPerformanceInformation() {
    int count = 24;
    std::string CurrentFrame = "Display Frequency: " + std::to_string( count );
    //TheEngine::Instance()->m_pTextRenderer->DrawTextString( CurrentFrame, 25.0f, (float)( TheEngine::Instance()->m_pWindow->GetBufferHeight() - 50.0f ), 0.5f, 0.9f, 0.9f, 0.9f );
    g_pTextRenderer->DrawTextString( CurrentFrame, 25.0f, 50.0f, 0.5f, 0.9f, 0.9f, 0.9f );
}