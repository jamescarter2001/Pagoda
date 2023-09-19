#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "mirage/core/buffer/pg_buffer.h"
#include "mirage/core/buffer/pg_vertex_buffer_layout.h"

namespace Pagoda::Mirage {
    class PAGODA_API IndexBuffer : public Buffer<int> {
    public:
        IndexBuffer(int buffer[], int bufferCount) : Buffer(buffer, bufferCount) { PG_CORE_DEBUG("Created index buffer: [BufferCount {}]", bufferCount); };
        virtual ~IndexBuffer() {};
    };
}