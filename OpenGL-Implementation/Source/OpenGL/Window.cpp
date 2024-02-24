#include "OpenGL/Window.h"

#include "Utility.h"

Window::Window() {
    m_pWindow = nullptr;
    m_BufferWidth = 0;
    m_BufferHeight = 0;
    m_Vsync = true;
}

Window::~Window() {
    if( m_pWindow != nullptr ) {
        glfwDestroyWindow( m_pWindow );
        m_pWindow = nullptr;
    }

    glfwTerminate();
}

bool Window::Initialization( unsigned int width, unsigned int height,
    const char *title, unsigned int windowNumber, GLFWwindow *firstWindow ) {
    /**************************************************************************/
    /*** Setup Main and Secondary Window the Same - Does not share context  ***/
    /*** items. Initalize glfw and glew once.                               ***/
    /**************************************************************************/
    if( windowNumber == 0 ) {
        if( glfwInit() == false ) {
            print_error_message( "ERROR: EXIT EARLY: GLFW Initialization failed." );
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

        // Lock current aspect ratio - Must be before window creation
        glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    }

    m_pWindow = glfwCreateWindow( width, height, title, nullptr, firstWindow );

    if( !m_pWindow ) {
        print_error_message( "ERROR: EXIT EARLY: GLFW main window creation failed." );
        glfwTerminate();
        return false;
    }

    // Get context for GLEW to use
    glfwMakeContextCurrent( m_pWindow );

    // Get buffer size information
    glfwGetFramebufferSize( m_pWindow, &m_BufferWidth, &m_BufferHeight );

    if( windowNumber == 0 ) {
        // Allow modern extension features
        glewExperimental = GL_TRUE;

        if( glewInit() != GLEW_OK ) {
            print_error_message( "ERROR: EXIT EARLY: GLEW main window initialization failed." );
            glfwDestroyWindow( m_pWindow );
            glfwTerminate();
            return false;
        }
    }

    // Setup Viewport Size
    glViewport( 0, 0, m_BufferWidth, m_BufferHeight );

    // Clear all the keys
    for( int i = 0; i < 1024; i++ ) {
        m_bKeys[i] = false;
    }

    // Handle Keys
    CreateCallbacks();

    // Tell window to stay open
    glfwSetWindowShouldClose( m_pWindow, GL_FALSE );

    // Set window position - Note: Rework to always draw playback window in top left corner,
    // and main window in bottom right corner.
    if( windowNumber == 0 ) {
        // Main Window
        glfwSetWindowPos( m_pWindow, int( m_BufferWidth * 0.50f ), int( m_BufferHeight * 0.05f ) );
    } else if( windowNumber == 1) {
        // Playback Window
        glfwSetWindowPos( m_pWindow, int( m_BufferWidth * 0.03f ), int( m_BufferHeight * 0.07f ) );
    } else {
        // GUI
        glfwSetWindowPos( m_pWindow, int( m_BufferWidth * 0.1f ), int( m_BufferHeight * 0.7f ) );
    }

    // Turn off vsync
    glfwSwapInterval( 0 );
 
    return true;
}

void Window::ToggleVsync() {
    m_Vsync = !m_Vsync;
    glfwSwapInterval( m_Vsync );
}

void Window::ClearColorBuffer() {
    glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );
}

void Window::CreateCallbacks() {
    // Setup GLFW callback that uses this object as owner
    // or else keys won't work
    glfwSetWindowUserPointer( m_pWindow, this );

    glfwSetKeyCallback( m_pWindow, HandleKeys );
    glfwSetInputMode( m_pWindow, GLFW_STICKY_KEYS, 1 );

    // Window resize callback
    //glfwSetFramebufferSizeCallback( m_pWindow, HandleFramebufferResize );
}

void Window::HandleKeys( GLFWwindow *window, int key, int code, int action, int mode ) {
    // Window pointer user function
    Window *theWindow = static_cast<Window *>( glfwGetWindowUserPointer( window ) );

    if( ( key == GLFW_KEY_ESCAPE ) && ( action == GLFW_PRESS ) ) glfwSetWindowShouldClose( window, GL_TRUE );
    //if( key == GLFW_KEY_R ) ResourceManager::ReloadShaders();
    //if( key == GLFW_KEY_T ) ReloadTextures();
    //if( key == GLFW_KEY_SPACE ) UpdatePlay();
    //if( key == GLFW_KEY_Z ) StepBackwards();
    //if( key == GLFW_KEY_X ) StepForwards();

    #ifdef _DEBUG
        // Note: Convert to Logger 
        if( ( key >= 0 ) && ( key < 1024 ) ) {
            if( action == GLFW_PRESS ) {
                theWindow->m_bKeys[key] = true;
                printf( "Pressed: %d\n", key );
            }
            else if( action == GLFW_RELEASE ) {
                theWindow->m_bKeys[key] = false;
                printf( "Released: %d\n", key );
            }
        }
    #endif
}

void Window::HandleFramebufferResize( GLFWwindow *window, int width, int height ) {
    glViewport( 0, 0, width, height );
}