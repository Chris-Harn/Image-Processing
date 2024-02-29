#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLenum glCheckError_( const char *file, int line );

void APIENTRY glDebugOutput( GLenum source,
                             GLenum type,
                             unsigned int id,
                             GLenum severity,
                             GLsizei length,
                             const char *message,
                             const void *userParam );

#endif

