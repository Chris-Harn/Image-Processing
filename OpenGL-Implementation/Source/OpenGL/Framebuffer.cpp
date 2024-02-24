#include "OpenGL/Framebuffer.h"

#include <GL/glew.h>

#include <stdio.h>

Framebuffer::Framebuffer() {
    m_FboID = 0;
    m_TextureID = 0;
    m_Width = 0;
    m_Height = 0;
}

Framebuffer::Framebuffer( unsigned int fbo_width, unsigned int fbo_height, bool texture ) {
    m_FboID = 0;
    m_TextureID = 0;
    m_Width = fbo_width;
    m_Height = fbo_height;

    glGenFramebuffers( 1, &m_FboID );
    glBindFramebuffer( GL_FRAMEBUFFER, m_FboID );  // GL_DRAW_FRAMEBUFFER (write ops), GL_READ_FRAMEBUFFER (read ops), GL_FRAMEBUFFER (both)

    glGenTextures( 1, &m_TextureID );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, m_TextureID );

    if( texture == false ) {
        // Will always normalize all values [0.0f, 1.0f] when shader finishes
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_SHORT, nullptr );
    } else {
        // Allows floats values to reside in texture
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, nullptr );
    }
    
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0 );

    GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
    if( status != GL_FRAMEBUFFER_COMPLETE ) {
        printf( "*** Framebuffer failed to be created. %i.\n", status );
    }

    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

Framebuffer::~Framebuffer() {
    if( m_TextureID != 0 ) {
        glDeleteTextures( 1, &m_TextureID );
        m_TextureID = 0;
    }

    if( m_FboID != 0 ) {
        glDeleteFramebuffers( 1, &m_FboID );
        m_FboID = 0;
    }
}

void Framebuffer::Bind() {
    glBindFramebuffer( GL_FRAMEBUFFER, m_FboID );
    glViewport( 0, 0, m_Width, m_Height );
}

void Framebuffer::Unbind() {
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void Framebuffer::BindTexture( unsigned int slot ) {
    glActiveTexture( GL_TEXTURE0 + slot );
    glBindTexture( GL_TEXTURE_2D, m_TextureID );
}

void Framebuffer::UnbindTexture( unsigned int slot ) {
    glBindTexture( GL_TEXTURE_2D, 0 + slot );
}