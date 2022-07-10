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
    void StartFrame();
    void DrawGui( ShaderControls &g_ShaderControls );
    void Draw();
};

#endif