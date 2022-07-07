#ifndef __GUI_H__
#define __GUI_H__

struct GLFWwindow;

class GUI {
public:
    GUI();
    ~GUI();
    void Initialization( GLFWwindow *mainWindow );
    void StartFrame();
    void DrawGui();
};

#endif