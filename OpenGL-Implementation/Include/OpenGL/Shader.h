#ifndef __SHADER_H__
#define __SHADER_H__

#include <glm/glm.hpp>

class Shader {
public:
    Shader();
    ~Shader();
    Shader &Use();

    bool Compile( const char *vertexCode, const char *fragmentCode, const char *geometryCode = nullptr );
    bool AddShader( unsigned int theProgram, const char *shaderCode, unsigned int shaderType );

    void SetFloat( const char *name, float value, bool useShader = false );
    void SetInteger( const char* name, int value, bool useShader = false );
    void SetBool( const char *name, bool value, bool useShader = false );
    void SetVec2( const char *name, const glm::vec2 &value, bool useShader = false );
    void SetVec2( const char *name, float x, float y, bool useShader = false );
    void SetVec3( const char *name, const glm::vec3 &value, bool useShader = false );
    void SetVec3( const char *name, float x, float y, float z, bool useShader = false );
    void SetVec4( const char *name, const glm::vec4 &value, bool useShader = false );
    void SetVec4( const char *name, float x, float y, float z, float w, bool useShader = false );
    void SetMat2( const char *name, const glm::mat2 &mat, bool useShader = false );
    void SetMat3( const char *name, const glm::mat3 &mat, bool useShader = false );
    void SetMat4( const char *name, const glm::mat4 &mat, bool useShader = false );

    unsigned int m_shaderID;
};

#endif