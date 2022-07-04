#ifndef __TEXTURE_H__
#define __TEXTURE_H__

class Texture {
public:
    Texture();
    ~Texture();

    void Generate( unsigned int width, unsigned int height, unsigned char* data);
    void Bind( unsigned int slot = 0 ) const;

public:
    unsigned int m_textureID;
    unsigned int m_width, m_height;
    unsigned int m_internalFormat;
    unsigned int m_imageFormat;
    unsigned int m_wrapS;
    unsigned int m_wrapT;
    unsigned int m_filterMin;
    unsigned int m_filterMax;
};

#endif

