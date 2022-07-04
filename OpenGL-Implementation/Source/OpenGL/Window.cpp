#include "OpenGL/Window.h"

#include <stdio.h>

Window::Window() {
    m_pMainWindow = nullptr;
    m_BufferWidth = 0;
    m_BufferHeight = 0;
    m_Vsync = true;
}

Window::~Window() {
    if( m_pMainWindow != nullptr ) {
        glfwDestroyWindow( m_pMainWindow );
        m_pMainWindow = nullptr;
    }

    glfwTerminate();
}

bool Window::Initialization( unsigned int width,
    unsigned int height,
    const char *title ) {
   
    if( glfwInit() == false ) {
        printf( "GLFW Initialization failed.\n" );
        glfwTerminate();
        return false;
    }

    // Setup GLFW window properties with OpenGL version
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 5 );
    // Core profile = No backwards compatibility and best performance
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    // Allow forward compatiblity
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );

    // Grab current window settings and make new window based on those settings
    //GLFWmonitor *monitor = glfwGetPrimaryMonitor(); // Read out all supported versions of this display
    //const GLFWvidmode *mode = glfwGetVideoMode( monitor );
    //m_pMainWindow = glfwCreateWindow( width, height, TITLE_RESOLUTION, glfwGetPrimaryMonitor(), nullptr );
    m_pMainWindow = glfwCreateWindow( width, height, title, nullptr, nullptr );
    //glfwSetWindowMonitor( m_pMainWindow, monitor, 0, 0, mode->width, mode->height, mode->refreshRate );

    if( !m_pMainWindow ) {
        printf( "GLFW window creation failed!" );
        glfwTerminate();
        return false;
    }
    
    // Get buffer size information
    glfwGetFramebufferSize( m_pMainWindow, &m_BufferWidth, &m_BufferHeight );

    // Get context for GLEW to use
    glfwMakeContextCurrent( m_pMainWindow );

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if( glewInit() != GLEW_OK ) {
        printf( "GLEW initialization failed!" );
        glfwDestroyWindow( m_pMainWindow );
        glfwTerminate();
        return false;
    }

    // Setup Viewport Size
    glViewport( 0, 0, m_BufferWidth, m_BufferHeight );

    // Tell window to stay open
    glfwSetWindowShouldClose( m_pMainWindow, GL_FALSE );

    return true;
}

void Window::ToggleVsync() {
    m_Vsync = !m_Vsync;
    glfwSwapInterval( m_Vsync );
}

void Window::ClearColorBuffer() {
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );
}