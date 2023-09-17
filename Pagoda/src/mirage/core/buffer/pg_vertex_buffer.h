#pragma once
#include "pgpch.h"

#include "mirage/core/buffer/pg_buffer.h"
#include "mirage/core/buffer/pg_vertex_buffer_layout.h"

namespace Pagoda::Mirage {
    class PAGODA_API VertexBuffer : public Buffer<float> {
    public:
        VertexBuffer(float* buffer, int count, VertexBufferLayout& layout) : Buffer(buffer, count), m_VertexBufferLayout(layout) {};
        virtual ~VertexBuffer() {};

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
    protected:
        VertexBufferLayout m_VertexBufferLayout;
    };
}