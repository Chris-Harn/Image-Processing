#ifndef __RESOURCEMANAGER_H__
#define __RESOURCEMANAGER_H__

#include <map>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "OpenGL/Texture.h"
#include "OpenGL/Shader.h"
#include "OpenGL/Framebuffer.h"

class ResourceManager {
public:
    // Resource Storage
    static std::map<std::string, std::string> m_ShaderFilePaths;
    static std::map<std::string, Shader*>      m_Shaders;
    static std::map<std::string, Framebuffer*> m_Framebuffers;
    static std::map<std::string, Texture*>     m_Textures;

    static Shader* LoadShader( const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name );
    static Shader* LoadShader( const char *ShaderFile, std::string name );
    static Shader* GetShader( std::string name );
    static Framebuffer* CreateFramebuffer( int width, int height, std::string name );
    static Framebuffer* GetFramebuffer( std::string name );
    static Texture* LoadTexture( const char *file, bool alpha, std::string name, bool flipImage );
    static Texture* GetTexture( std::string name );
    static void CleanUp(); // properly de-allocates all loaded resources

    static void ReloadShaders();
private:
    ResourceManager() { }  // Singleton
    
    static Shader* LoadShaderFromFiles( const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr );
    static Shader* LoadShaderFromFile( const char *shaderFile );
    static Texture* LoadTextureFromFile( const char *file, bool alpha, bool flipImage );
};

#endif