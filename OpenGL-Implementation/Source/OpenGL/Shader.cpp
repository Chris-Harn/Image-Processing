#include "OpenGL/Shader.h"

#include <string>
#include <stdio.h>  // printf
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

enum class ShaderType {
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1
};

Shader::Shader() {
    m_shaderID = 0;
}

Shader::~Shader() {
    if( m_shaderID != 0 ) glDeleteProgram( m_shaderID );
}

Shader &Shader::Use() {
    glUseProgram( this->m_shaderID );
    return *this;
}

bool Shader::Compile( const char *vertexCode, const char *fragmentCode, const char *geometryCode ) {
    // Create program, and give shader the id from the GPU
    m_shaderID = glCreateProgram();

    if( !m_shaderID ) {
        printf( "Error creating shader program!\n" );
        return false;
    }

    // Compile vertex shader and add to program
    if( AddShader( m_shaderID, vertexCode, GL_VERTEX_SHADER ) == false ) return false;

    // Compile fragment shader and add to program
    if( AddShader( m_shaderID, fragmentCode, GL_FRAGMENT_SHADER ) == false ) return false;

    if( geometryCode != nullptr ) {
        // Compile fragment shader and add to program
        AddShader( m_shaderID, geometryCode, GL_GEOMETRY_SHADER );
    }

    // Check error information for compiling shader programs
    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    // Create program on the GPU and link
    glLinkProgram( m_shaderID );
    // Check program was correctly linked
    glGetProgramiv( m_shaderID, GL_LINK_STATUS, &result );

    if( !result ) {
        glGetProgramInfoLog( m_shaderID, sizeof( eLog ), NULL, eLog );
        printf( "Error linking program: '%s'\n", eLog );
        return false;
    }

    // Check if shader created is valid in the current version/context
    // of opengl
    glGetProgramiv( m_shaderID, GL_VALIDATE_STATUS, &result );

    if( result ) {
        glGetProgramInfoLog( m_shaderID, sizeof( eLog ), NULL, eLog );
        printf( "Error validating program: '%s'\n", eLog );
        return false;
    }

    return true;
}

bool Shader::AddShader( unsigned int theProgram, const char *shaderCode, unsigned int shaderType ) {
    // Create empty shader for our type: vertex or fragment shader
    GLuint theShader = glCreateShader( shaderType );

    // Create array pointing at start of code.
    const GLchar *theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = (int)strlen( shaderCode );

    glShaderSource( theShader, 1, theCode, codeLength );
    glCompileShader( theShader );

    // Check error information for compiling shader programs
    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    // Check program was correctly compiled
    glGetShaderiv( theShader, GL_COMPILE_STATUS, &result );

    if( !result ) {
        glGetShaderInfoLog( theShader, sizeof( eLog ), NULL, eLog );
        printf( "Error compiling the %d shader: '%s'\n", shaderType, eLog );
        return false;
    }

    // attach shader to the program in the GPU
    glAttachShader( theProgram, theShader );

    glDeleteShader( theShader );

    return true;
}

void Shader::SetFloat( const char *name, float value, bool useShader ) {
    if( useShader )
        this->Use();
    glUniform1f(glGetUniformLocation( this->m_shaderID, name), value );
}

void Shader::SetInteger( const char *name, int value, bool useShader ) {
    if( useShader )
        this->Use();
    glUniform1i( glGetUniformLocation( this->m_shaderID, name ), value );
}

void Shader::SetBool( const char *name, bool value, bool useShader ) {
    if( useShader )
        this->Use();
    glUniform1i( glGetUniformLocation( this->m_shaderID, name ), (int)value );
}

void Shader::SetVec2( const char *name, const glm::vec2 &value, bool useShader ) {
    if( useShader )
        this->Use();
    glUniform2fv( glGetUniformLocation( this->m_shaderID, name ), 1, &value[0] );
}

void Shader::SetVec2( const char *name, float x, float y, bool useShader ) {
    if( useShader )
        this->Use();
    glUniform2f( glGetUniformLocation( m_shaderID, name ), x, y );
}

void Shader::SetVec3( const char *name, const glm::vec3 &value, bool useShader ) {
    if( useShader )
        this->Use();
    glUniform3fv( glGetUniformLocation( m_shaderID, name ), 1, &value[0] );
}

void Shader::SetVec3( const char *name, float x, float y, float z, bool useShader ) {
    if( useShader )
        this->Use();
    glUniform3f( glGetUniformLocation( m_shaderID, name ), x, y, z );
}

void Shader::SetVec4( const char *name, const glm::vec4 &value, bool useShader ) {
    if( useShader )
        this->Use();
    glUniform4fv( glGetUniformLocation( m_shaderID, name ), 1, &value[0] );
}

void Shader::SetVec4( const char *name, float x, float y, float z, float w, bool useShader ) {
    if( useShader )
        this->Use();
    glUniform4f( glGetUniformLocation( m_shaderID, name ), x, y, z, w );
}

void Shader::SetMat2( const char *name, const glm::mat2 &mat, bool useShader ) {
    if( useShader )
        this->Use();
    glUniformMatrix2fv( glGetUniformLocation( m_shaderID, name ), 1, GL_FALSE, &mat[0][0] );
}

void Shader::SetMat3( const char *name, const glm::mat3 &mat, bool useShader ) {
    if( useShader )
        this->Use();
    glUniformMatrix3fv( glGetUniformLocation( m_shaderID, name ), 1, GL_FALSE, &mat[0][0] );
}

void Shader::SetMat4( const char *name, const glm::mat4 &mat, bool useShader ) {
    if( useShader )
        this->Use();
    glUniformMatrix4fv( glGetUniformLocation( m_shaderID, name ), 1, GL_FALSE, &mat[0][0] );
}