#include "OpenGL/ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "stb_image.h"

// Initialize static variables
std::map<std::string, std::string>    ResourceManager::m_ShaderFilePaths;
std::map<std::string, Texture*>       ResourceManager::m_Textures;
std::map<std::string, Shader*>        ResourceManager::m_Shaders;
std::map<std::string, Framebuffer*>   ResourceManager::m_Framebuffers;
std::map<std::string, Framebuffer*>   ResourceManager::m_LastFramebuffer;

enum class ShaderType {
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1,
    GEOMETRY = 2
};

Shader* ResourceManager::LoadShader( const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name ) {
    Shader* temp = nullptr;
    temp = LoadShaderFromFiles( vShaderFile, fShaderFile, gShaderFile );

    if(temp == nullptr) {
        printf("Failed to compile %s.\n", name.c_str() );
        return m_Shaders[name] = new Shader( *m_Shaders["CautionImage"] );
    } 

    return m_Shaders[name] = temp;
}

Shader* ResourceManager::LoadShader( const char *ShaderFile, std::string name ) {
    // Save filepath for when needing to reload shaders
    std::string string_input( ShaderFile );

    // If the key and path doesn't doesn't already exist... save it to reload using
    if( !m_ShaderFilePaths.count( name ) ) {
        m_ShaderFilePaths.insert( std::pair<std::string, std::string>( name, string_input ) );
    }
    
    Shader *temp = nullptr;
    temp = LoadShaderFromFile( ShaderFile );

    if( temp == nullptr ) {
        printf( "Failed to compile %s.\n", name.c_str() );
        return m_Shaders[name] = new Shader( *m_Shaders["CautionImage"] );
    }

    return m_Shaders[name] = temp;
}

Shader* ResourceManager::GetShader( std::string name ) {
    return m_Shaders[name];
}

Framebuffer* ResourceManager::CreateFramebuffer( int width, int height, std::string name, bool texture ) {
    Framebuffer *newFrameBuffer = new Framebuffer( width, height, texture );

    m_Framebuffers[name] = newFrameBuffer;
    return m_Framebuffers[name];
}

Framebuffer* ResourceManager::GetFramebuffer( std::string name ) {
    return m_Framebuffers[name];
}

void ResourceManager::SaveLastFramebuffer( std::string name ) {
    // Clear last shader
    m_LastFramebuffer.clear();

    m_LastFramebuffer[name] = m_Framebuffers[name];
}

Framebuffer* ResourceManager::GetLastFramebuffer() {
    for( auto iter : m_LastFramebuffer ) return iter.second;

    return nullptr;
}

Texture* ResourceManager::LoadTexture( const char *file, bool alpha, std::string name, bool flipImage ) {
    m_Textures[name] = LoadTextureFromFile(file, alpha, flipImage );
    return m_Textures[name];
}

Texture* ResourceManager::GetTexture( std::string name ) {
    return m_Textures[name];
}

void ResourceManager::CleanUp() {
    // delete all shaders
    for( auto iter : m_Shaders ) delete iter.second;
    m_Shaders.clear();
    // delete all paths to reload shaders
    m_ShaderFilePaths.clear();
    // delete all framebuffers
    for( auto iter : m_Framebuffers ) delete iter.second;
    m_Framebuffers.clear();
    // delete all textures
    for( auto iter : m_Textures ) delete iter.second;
    m_Textures.clear();
}

Shader* ResourceManager::LoadShaderFromFiles( const char *vShaderFile, const char *fShaderFile, const char *gShaderFile ) {
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try {
        // Open Files
        std::ifstream vertexShaderFile( vShaderFile );
        std::ifstream fragementShaderFile( fShaderFile );
        std::stringstream vShaderStream, fShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragementShaderFile.rdbuf();
        // Close file handlers
        vertexShaderFile.close();
        fragementShaderFile.close();
        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // If geometry shader path is present, also load a geometry shader
        if( gShaderFile != nullptr ) {
            std::ifstream geometryShaderFile( gShaderFile );
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
        
    } catch ( std::exception e ) {
        printf( "ERROR::SHADER: Failed to read shader files.\n" );
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();

    Shader* shader = new Shader();
    if( shader->Compile( vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode: nullptr ) == false) {
        return nullptr;
    }
    return shader;
}

Shader* ResourceManager::LoadShaderFromFile( const char *shaderFile ) {
    std::stringstream ss[3];
    bool foundGeometryShader = false;

    try {
        // Open a stream to read from the file
        std::ifstream stream( shaderFile );
        std::string line;

        ShaderType type = ShaderType::NONE;

        while( getline( stream, line ) ) {
            // #shader in a line is defining the type of shader
            if( line.find( "#shader" ) != std::string::npos ) {
                // #shader vertex
                if( line.find( "vertex" ) != std::string::npos ) {
                    type = ShaderType::VERTEX;
                }
                // #shader fragment
                if( line.find( "fragment" ) != std::string::npos ) {
                    type = ShaderType::FRAGMENT;
                }
                // #shader geometry
                if( line.find( "geometry" ) != std::string::npos ) {
                    type = ShaderType::GEOMETRY;
                    foundGeometryShader = true;
                }
            }
            else {
                if( type != ShaderType::NONE ) {
                    ss[(int)type] << line << '\n';
                }
                else {
                    // Error state, return null
                    std::cout << "Didn't recieve shader type." << std::endl;
                }
            }
        }
    }
    catch( std::exception e ) {
        printf( "ERROR::SHADER: Failed to read shader files.\n" );
    }
    std::string vertexCode = ss[(int)ShaderType::VERTEX].str();
    std::string fragmentCode = ss[(int)ShaderType::FRAGMENT].str();
    std::string geometryCode = ss[(int)ShaderType::GEOMETRY].str();

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();

    Shader* shader = new Shader();
    if( shader->Compile( vShaderCode, fShaderCode, foundGeometryShader == true ? gShaderCode : nullptr ) == false) {
     return nullptr;   
    }
    return shader;
}

Texture* ResourceManager::LoadTextureFromFile( const char *file, bool alpha, bool flipImage ) {
    Texture* texture = new Texture();

    if( alpha == true ) {
        texture->m_internalFormat = GL_RGBA;
        texture->m_imageFormat = GL_RGBA;
    }
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load( flipImage ); // Flip if needed, but will restore afterwards
    unsigned char *data = stbi_load( file, &width, &height, &nrChannels, 0 );
    texture->Generate( width, height, data );
    
    // Free Image Data
    stbi_image_free(data);

    stbi_set_flip_vertically_on_load( false ); // Restore

    return texture;
}

void ResourceManager::ReloadShaders() {
    printf("Reloading shaders.\n");

    // Delete Previous Shaders
    for( auto iter : m_Shaders ) delete iter.second;
    m_Shaders.clear();

    // Reload same shaders
    for( auto iter : m_ShaderFilePaths ) {
        LoadShader( iter.second.c_str(), iter.first );
    }
}