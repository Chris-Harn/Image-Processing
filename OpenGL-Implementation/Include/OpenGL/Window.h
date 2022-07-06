#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
public:
    Window();
    ~Window();
    bool Initialization( unsigned int width,
                         unsigned int height,
                         const char *title,
                         bool secondary_window, 
                         GLFWwindow *firstWindow );
    float GetBufferWidth() { return (float)m_BufferWidth; }
    float GetBufferHeight() { return (float)m_BufferHeight; }
    bool GetShouldClose() { return glfwWindowShouldClose( m_pWindow ); }
    void CloseTheWindow() { glfwSetWindowShouldClose( m_pWindow, GL_TRUE ); }
    void PollEvents() { glfwPollEvents(); }
    bool *GetsKeys() { return m_bKeys; }
    void ToggleVsync();
    void ClearColorBuffer();
    void SwapBuffers() { glfwSwapBuffers( m_pWindow ); }
    GLFWwindow *GetWindow() { return m_pWindow; }
    void MakeCurrentContext() { glfwMakeContextCurrent( m_pWindow ); }

private:
    GLFWwindow *m_pWindow;
    int m_BufferWidth;
    int m_BufferHeight;
    bool m_Vsync;
    bool m_bKeys[1024];

    // Callback function require static functions
    void CreateCallbacks();
    static void HandleKeys( GLFWwindow *window, int key, int code, int action, int mode );
    static void HandleFramebufferResize( GLFWwindow *window, int width, int height );
};

#endif
