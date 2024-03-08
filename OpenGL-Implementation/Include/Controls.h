#ifndef __CONTROL_H__
#define __CONTROL_H__

// NOTE: Hard coding what is on right now, but will eventually
// move into xml files

#include <string>

struct ProgramControls {
    // Path to Video and wither it has been processed
    bool m_bprocessNewVideo = false;
    std::string m_spathToCurrentVideo = "../../VHS_1980.avi";

    // One spot for all the ways to close to communicate using
    bool m_bcloseProgram = false;

    // Change Picture Orientation
    bool m_bflipHorizontal = false;
    bool m_bflipVertical = true;

    // Display FPS
    bool m_bDisplayFPS = true;

    // ******* Convolution Filters *******
    // Color Median Filter
    bool m_bcolorMedian = true;

    // Low Pass Filter
    bool m_bguassianBlur = true;

    // Simple Sharpen Filter
    bool m_bsharpeningPass = true;

    // Anti-Aliasing Filter
    bool m_bAntiAliasing = true;

    // ****** Filters
    // Average Frames
    bool m_bAverageFrame = false;
    
    // Input Gamma Correction
    bool m_binputGamma = true;
    float m_inputGamma = 1.314f;

    // Output Gamma Correction
    bool m_boutputGamma = true;
    float m_outputGamma = 0.987f;

    // Saturation Enhance
    bool m_bEnhanceSaturation = true;
    float m_enhanceSaturation = 1.0f;

    // ******* Histogram Manipulation *******
    bool m_bHistoramSpread = true;
    float m_histogram[512];
    float m_stretchHistogram[512];
    float m_backProjection[512];

    // ******* Image Upscalers *******
    int m_upscalerSelection = 3;
};

#endif