#ifndef __CONTROL_H__
#define __CONTROL_H__

// NOTE: Hard coding what is on right now, but will eventually
// move into xml files

struct ShaderControls {
    // One spot for all the ways to close to communicate using
    bool m_bcloseProgram;

    // Change Picture Orientation
    bool m_bflipHorizontal;
    bool m_bflipVertical = true;

    // Input Gamma Correction
    bool m_binputGamma;
    float m_inputGamma;

    // Convolution Filter 

    // Low Pass Filter
    bool m_bguassianBlur = true;

    // Simple Sharpen Filter
    bool m_bsharpeningPass = true;

    // Output Gamma Correction
    bool m_boutputGamma;
    float m_outputGamma;
};

#endif