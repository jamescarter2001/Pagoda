#pragma once
#include "pgpch.h"

#include "pgBuffer.h"

namespace Pagoda::Mirage {
    class PAGODA_API VertexBuffer : public Buffer<void*> {
    public:
        static VertexBuffer* Create(void* data, const unsigned int size);

        virtual ~VertexBuffer() {}

    protected:
        VertexBuffer(void* data, const unsigned int size) : Buffer(data, size) {}
    }; 
}