#include "OpenGL/ImageFilter.h"

ImageFilter::ImageFilter() {
    m_bActiveFilter = false;
    m_bDirty = true;
    m_textureID = 0;
    m_width = 0;
    m_height = 0;
}

ImageFilter::~ImageFilter() {

}

void ImageFilter::Process( unsigned int inputImage ) {
    if( ( m_bActiveFilter == false ) || ( m_bDirty == true ) ) {
        return; // Nothing to process
    }

    // Process input image here.... 
}

unsigned int ImageFilter::Image() {
    return m_textureID;
}

int ImageFilter::Width() {
    return m_width;
}

int ImageFilter::Height() {
    return m_height;
}

void ImageFilter::SetSize( int width, int height ) {
    m_width = width;
    m_height = height;
}