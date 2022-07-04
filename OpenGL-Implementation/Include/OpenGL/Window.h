#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
public:
    Window();
    ~Window();
    bool Initialization( unsigned int width,
                         unsigned int height, 
                         const char* title );
    float GetBufferWidth() { return (float)m_BufferWidth; }
    float GetBufferHeight() { return (float)m_BufferHeight; }
    bool GetShouldClose() { return glfwWindowShouldClose( m_pWindow ); }
    void CloseTheWindow() { glfwSetWindowShouldClose( m_pWindow, GL_TRUE ); }
    void ToggleVsync();
    void ClearColorBuffer();
    void SwapBuffers() { glfwSwapBuffers( m_pWindow ); }
    GLFWwindow *GetWindow() { return m_pWindow; }

private:
    GLFWwindow *m_pWindow;
    int m_BufferWidth;
    int m_BufferHeight;
    bool m_Vsync;
};

#endif
