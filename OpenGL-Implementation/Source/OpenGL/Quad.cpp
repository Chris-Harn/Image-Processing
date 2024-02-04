#include "OpenGL/Quad.h"

#include <GL/glew.h>

Quad::Quad() {
    unsigned short Qindices[] = {
    0, 1, 2, // triangle 1
    0, 2, 3 // triangle 2
    };

    // draw to screen
    GLfloat Qvertices[] = {
        // x,     y,    u,     v
        -1.0f,  1.0f,  0.0f,  1.0f,
        -1.0f, -1.0f,  0.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  1.0f
    };

    m_indexCount = 16;
    m_numOfVertices = 16;
    m_numOfIndices = 6;

    glGenVertexArrays( 1, &m_VAO );
    glBindVertexArray( m_VAO );

    glGenBuffers( 1, &m_IBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( Qindices[0] ) * m_numOfIndices, Qindices, GL_STATIC_DRAW );

    glGenBuffers( 1, &m_VBO );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );

    glBufferData( GL_ARRAY_BUFFER, sizeof( Qvertices[0] ) * m_numOfVertices, Qvertices, GL_STATIC_DRAW );

    // layout = 0, vertex position
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, sizeof( Qvertices[0] ) * 4, (void *)( 0 ) );
    glEnableVertexAttribArray( 0 );

    // layout = 1, texture coordinates
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( Qvertices[0] ) * 4, (void *)( sizeof( Qvertices[0] ) * 2 ) );
    glEnableVertexAttribArray( 1 );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

    glBindVertexArray( 0 );
}

Quad::~Quad() {
    if( m_VBO != 0 ) {
        // delete buffer off GPU
        glDeleteBuffers( 1, &m_VBO );
        m_VBO = 0;
    }

    if( m_IBO != 0 ) {
        // delete buffer off GPU
        glDeleteBuffers( 1, &m_IBO );
        m_IBO = 0;
    }

    if( m_VAO != 0 ) {
        // delete buffer off GPU
        glDeleteVertexArrays( 1, &m_VAO );
        m_VAO = 0;
    }

    m_indexCount = 0;
}

void Quad::RenderQuad() {
    glBindVertexArray( m_VAO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IBO );
    glDrawElements( GL_TRIANGLES, m_indexCount, GL_UNSIGNED_SHORT, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
}
