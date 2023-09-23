#include "pgpch.h"

#include "pg_gl_index_buffer.h"

namespace Pagoda::Mirage {
    GLIndexBuffer::GLIndexBuffer(int data[], int count) : IndexBuffer::IndexBuffer(data, count) {
        glGenBuffers(1, &this->m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, data, GL_STATIC_DRAW);
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