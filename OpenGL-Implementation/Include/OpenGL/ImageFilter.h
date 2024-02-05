#ifndef __IMAGEFILTER_H__
#define __IMAGEFILTER_H__

class ImageFilter {
public:
    ImageFilter();
    ~ImageFilter();

    void Process( unsigned int inputImage );

    unsigned int Image(); // Return output image
    int Width();
    int Height();
    void SetSize( int width, int height );

private:
    bool m_bActiveFilter;
    bool m_bDirty;
    unsigned int m_textureID;

    int m_width;
    int m_height;
};

#endif
