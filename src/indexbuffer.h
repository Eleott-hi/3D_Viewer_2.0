#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include "glwidget.h"

class IndexBuffer : public GLWidget {
   private:
    unsigned int m_RendererID;
    unsigned int m_Count;

   public:
    IndexBuffer(unsigned int count, unsigned int *data);
    ~IndexBuffer();

    void Bind();
    void Unbind();
};

#endif  // INDEXBUFFER_H
