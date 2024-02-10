#include "Application.h"

#include <string>

void DisplayPerformanceInformation() {
    int count = 24;
    std::string CurrentFrame = "Display Frequency: " + std::to_string( count );
    TheApplication::Instance()->g_pTextRenderer->DrawTextString( CurrentFrame, 25.0f, (float)( TheApplication::Instance()->g_pMainWindow->GetBufferHeight() - 50.0f ), 0.5f, 0.9f, 0.9f, 0.9f );
}