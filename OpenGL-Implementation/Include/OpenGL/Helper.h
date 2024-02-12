#include "Application.h"

#include <string>

void DisplayPerformanceInformation() {
    int count = TheApplication::Instance()->m_pAppTimer->GetFPS();
    std::string CurrentFrame = "Display Frequency: " + std::to_string( count );
    TheApplication::Instance()->m_pTextRenderer->DrawTextString( CurrentFrame, 25.0f, (float)( TheApplication::Instance()->m_pSecondaryWindow->GetBufferHeight() - 50.0f ), 0.5f, 0.9f, 0.9f, 0.9f );
}