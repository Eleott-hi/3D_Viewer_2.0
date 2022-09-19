#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H
#include "glwidget.h"

class VertexBuffer : public GLWidget
{

private:
    unsigned int m_RendererID;

public:
    VertexBuffer(unsigned int size, void *data);
    ~VertexBuffer();

    void Bind();
    void Unbind();
};

#endif // VERTEXBUFFER_H
