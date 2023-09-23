#pragma once

#include <glad/glad.h>

#include "mirage/core/buffer/pg_vertex_buffer.h"
#include "mirage/core/buffer/pg_vertex_buffer_layout.h"

namespace Pagoda::Mirage {
    class GLVertexArray {
    public:
        GLVertexArray();
        ~GLVertexArray();

        void Bind() const;
        void Unbind() const;

        void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    private:
        unsigned int m_RendererID;
    };
}