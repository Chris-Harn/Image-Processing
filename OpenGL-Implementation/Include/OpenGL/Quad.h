#ifndef _QUAD_H_
#define _QUAD_H_

class Quad {
public:
    Quad();
    ~Quad();
    void RenderQuad();

private:
    unsigned int m_VAO, m_VBO, m_IBO;
    int m_indexCount;
    int m_numOfIndices;
    int m_numOfVertices;
};

#endif