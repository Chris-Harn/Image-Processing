#ifndef __GUI_H__
#define __GUI_H__

#include "Controls.h"

struct GLFWwindow;
struct ShaderControls;

class GUI {
public:
    GUI();
    ~GUI();
    void Initialization( GLFWwindow *mainWindow );
    void PollGuiEvents( ProgramControls &g_ProgramControls );
    void Draw();
};

#endif