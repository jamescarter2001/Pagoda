#pragma once
#include "pgpch.h"

#include "mirage/core/buffer/pg_buffer.h"

namespace Pagoda::Mirage {
    class PAGODA_API VertexBuffer : public Buffer<float> {
    public:
        VertexBuffer(float* buffer, int count) : Buffer(buffer, count) {};
        virtual ~VertexBuffer() {};
    };
}