#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include "OpenGL/Texture.h"

class Framebuffer {
public:
    Framebuffer();
    Framebuffer( unsigned int fbo_width, unsigned int fbo_height );
    ~Framebuffer();

    void Bind();
    void Unbind();
    void BindTexture( unsigned int slot = 0 );
    void UnbindTexture( unsigned int slot = 0 );
    unsigned int GetTextureID() { return m_TextureID; }
    unsigned int GetFBOID() { return m_FboID; }
private:
    unsigned int m_TextureID;
    unsigned int m_FboID;
    unsigned int m_Width;
    unsigned int m_Height;
};

#endif