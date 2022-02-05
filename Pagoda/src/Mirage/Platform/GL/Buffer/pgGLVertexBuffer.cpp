#include "pgpch.h"

#include "pgGLVertexBuffer.h"

namespace Pagoda::Mirage {
    VertexBuffer* VertexBuffer::Create(void* data, const unsigned int size) {
        return new GLVertexBuffer(data, size);
    }

    GLVertexBuffer::GLVertexBuffer(void* data, const unsigned int size) : VertexBuffer::VertexBuffer(data, size) {
        glGenBuffers(1, &this->m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, this->m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    GLVertexBuffer::~GLVertexBuffer() {
        glDeleteBuffers(1, &this->m_RendererID);
    }

    void GLVertexBuffer::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, this->m_RendererID);
    }

    void GLVertexBuffer::Unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}