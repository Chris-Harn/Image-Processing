#ifndef __CONTROL_H__
#define __CONTROL_H__

struct ShaderControls {
    bool m_bcloseProgram;

    bool m_bflipHorizontal;
    bool m_bflipVertical;

    bool m_binputGamma;
    float m_inputGamma;

    bool m_boutputGamma;
    float m_outputGamma;
};

#endif