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

    // Input Gamma Correction
    bool m_binputGamma = false;
    float m_inputGamma = 1.0f;

    // Convolution Filter 

    // Low Pass Filter
    bool m_bguassianBlur = true;

    // Simple Sharpen Filter
    bool m_bsharpeningPass = true;

    // Output Gamma Correction
    bool m_boutputGamma = false;
    float m_outputGamma = 1.0f;
};

#endif