#include "pgpch.h"

#include "pgGLIndexBuffer.h"

namespace Pagoda::Mirage {
    IndexBuffer* IndexBuffer::Create(unsigned int* data, const unsigned int size) {
        return new GLIndexBuffer(data, size);
    }

    GLIndexBuffer::GLIndexBuffer(unsigned int* data, const unsigned int size) : IndexBuffer::IndexBuffer(data, size) {
        glGenBuffers(1, &this->m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    GLIndexBuffer::~GLIndexBuffer() {
        glDeleteBuffers(1, &this->m_RendererID);
    }

    void GLIndexBuffer::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_RendererID);
    }

    void GLIndexBuffer::Unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}