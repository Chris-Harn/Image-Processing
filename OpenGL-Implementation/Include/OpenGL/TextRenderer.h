#ifndef __TEXTRENDERER_H__
#define __TEXTRENDERER_H__

#include <glm/glm.hpp> // glm::vec2, glm::vec3, glm::mat4

#include <GL/glew.h>

#include <string> // std::string
#include <vector> // std::vector
#include <map>    // std::map

struct Character;
class Shader;
class Window;

class TextRenderer {
public:
    TextRenderer();
    ~TextRenderer();
    void Initialize( Window *window );

    void DrawTextString( std::string text, float x, float y, float scale, float red, float green, float blue );
    enum PROTO {
        ARRAY_LIMIT = 200 /* Dev choosen number */
    };
    void TextRenderCall( int length );
    void Clean();
private:
    std::map<char, Character> Characters;
    Window *m_pWindow;
    unsigned int VAO;
    unsigned int VBO;
    GLuint m_textureArray;
    std::vector<glm::mat4> m_transforms;
    std::vector<int> letterMap;
};

#endif