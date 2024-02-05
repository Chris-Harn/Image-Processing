#include "OpenGL/TextRenderer.h"

#include "OpenGL/Window.h"
#include "OpenGL/Shader.h"
#include "OpenGL/ResourceManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::scale

#include <stdio.h>

struct Character {
    int TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    unsigned int Advance;
};

TextRenderer::TextRenderer() {
    m_pWindow = nullptr;
    VAO = 0;
    VBO = 0;
    m_textureArray = 0;

    FT_Library ft;
    if( FT_Init_FreeType( &ft ) ) {
        printf( "ERROR::FREETYPE: Could not init FreeType Library.\n" );
        return;
    }

    FT_Face face;
    if( FT_New_Face( ft, "Resource/Font/arial.ttf", 0, &face ) ) {
        printf( "ERROR::FREETYPE: Failed to load font.\n" );
        return;
    }

    FT_Set_Pixel_Sizes( face, 256, 256 );

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    glGenTextures( 1, &m_textureArray );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D_ARRAY, m_textureArray );
    glTexImage3D( GL_TEXTURE_2D_ARRAY, 0, GL_R8, 256, 256, 128, 0, GL_RED, GL_UNSIGNED_BYTE, 0 );

    for( unsigned char c = 0; c < 128; c++ ) {
        if( FT_Load_Char( face, c, FT_LOAD_RENDER ) ) {
            printf( "ERROR::FREETYPE: Failed to load Glyph.\n" );
            return;
        }

        glTexSubImage3D(
            GL_TEXTURE_2D_ARRAY,
            0, 0, 0, int( c ),
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows, 1,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        // Set texture options
        glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        Character character = {
           int( c ),
           glm::ivec2( face->glyph->bitmap.width, face->glyph->bitmap.rows ),
           glm::ivec2( face->glyph->bitmap_left, face->glyph->bitmap_top ),
           face->glyph->advance.x
        };

        Characters.insert( std::pair<char, Character>( c, character ) );
    }

    glBindTexture( GL_TEXTURE_2D_ARRAY, 0 );

    FT_Done_Face( face );
    FT_Done_FreeType( ft );

    for( int i = 0; i < ARRAY_LIMIT; i++ ) {
        letterMap.push_back( 0 );
        m_transforms.push_back( glm::mat4( 1.0f ) );
    }
}

TextRenderer::~TextRenderer() {
    // Never clear OpenGL resources in deconstructor
}

void TextRenderer::Clean() {
    // None of these are originals so set to nullprt
    m_pWindow = nullptr;

    if( VBO != 0 ) {
        // delete buffer off GPU
        glDeleteBuffers( 1, &VBO );
        VBO = 0;
    }

    if( VAO != 0 ) {
        // delete buffer off GPU
        glDeleteVertexArrays( 1, &VAO );
        VAO = 0;
    }
}

GLfloat vertex_data[] = {
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 0.0f
};

void TextRenderer::Initialize( Window *window ) {
    m_pWindow = window;

    // Set up VAO/VBO for text rendering
    glGenVertexArrays( 1, &VAO );
    glBindVertexArray( VAO );
    glGenBuffers( 1, &VBO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertex_data ), vertex_data, GL_STATIC_DRAW );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );

    // Convert from NDC space to coordinate system that is the window in pixels
    glm::mat4 projection = glm::ortho( 0.0f, static_cast<float>( m_pWindow->GetBufferWidth() ), 0.0f, static_cast<float>( m_pWindow->GetBufferHeight() ) );

    // Send projection matrix once as screen will not change screen size
    ResourceManager::GetShader( "FastBlitText" )->SetMat4( "projection", projection, true );
}

void TextRenderer::DrawTextString( std::string text, float x, float y, float scale, float red, float green, float blue ) {
    glEnable( GL_CULL_FACE );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    scale = scale * 48.0f / 256.0f;

    glm::vec3 color = glm::vec3( red, green, blue );

    // activate corresponding render state	
    float copyX = x;

    ResourceManager::GetShader( "FastBlitText" )->SetVec3( "textColor", color, true );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D_ARRAY, m_textureArray );
    glBindVertexArray( VAO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );

    int workingIndex = 0;

    // iterate through all characters
    std::string::const_iterator c;
    for( c = text.begin(); c != text.end(); c++ ) {
        Character ch = Characters[*c];

        if( *c == '\n' ) {
            // Skip to next line and skip a draw call
            y -= ( ( ch.Size.y ) ) * 1.3 * scale;
            x = copyX;
        }
        else if( *c == ' ' ) {
            // Move forward and skip a draw call
            x += ( ch.Advance >> 6 ) * scale;
        }
        else {
            float xpos = x + ch.Bearing.x * scale;
            float ypos = y - ( 256 - ch.Bearing.y ) * scale;

            m_transforms[workingIndex] = glm::translate( glm::mat4( 1.0f ), glm::vec3( xpos, ypos, 0 ) ) * glm::scale( glm::mat4( 1.0f ), glm::vec3( 256 * scale, 256 * scale, 0 ) );
            letterMap[workingIndex] = ch.TextureID;

            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += ( ch.Advance >> 6 ) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)

            // Only increment characters we are drawing. Not spaces and new lines
            workingIndex++;

            if( workingIndex == ARRAY_LIMIT - 1 ) {
                TextRenderCall( workingIndex );
                workingIndex = 0;
            }
        }
    }

    // Send any remaining words
    TextRenderCall( workingIndex );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
    glBindTexture( GL_TEXTURE_2D_ARRAY, 0 );

    glUseProgram( 0 );

    glDisable( GL_CULL_FACE );
    glDisable( GL_BLEND );
}

void TextRenderer::TextRenderCall( int length ) {
    if( length != 0 ) {
        // Can't use Resource manager as we are instancing the draw calls
        glUniformMatrix4fv( glGetUniformLocation( ResourceManager::GetShader( "FastBlitText" )->m_shaderID, "transforms" ), length, GL_FALSE, &m_transforms[0][0][0] );
        glUniform1iv( glGetUniformLocation( ResourceManager::GetShader( "FastBlitText" )->m_shaderID, "letterMap" ), length, &letterMap[0] );
        glDrawArraysInstanced( GL_TRIANGLE_STRIP, 0, 4, length );
    }
}