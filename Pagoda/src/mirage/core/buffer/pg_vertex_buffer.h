#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "mirage/core/buffer/pg_buffer.h"
#include "mirage/core/buffer/pg_vertex_buffer_layout.h"

namespace Pagoda::Mirage {
    class PAGODA_API VertexBuffer : public Buffer<float> {
    public:
        VertexBuffer(float buffer[], int size, int vertexCount, VertexBufferLayout layout) : Buffer(buffer, size), m_VertexCount(vertexCount), m_VertexBufferLayout(layout) { PG_CORE_DEBUG("Created vertex buffer: [Size {} : VertexCount {}]", size, vertexCount); };
        virtual ~VertexBuffer() {};

        inline int GetVertexCount() {
            return m_VertexCount;
        }

    protected:
        int m_VertexCount;
        VertexBufferLayout m_VertexBufferLayout;
    };
}