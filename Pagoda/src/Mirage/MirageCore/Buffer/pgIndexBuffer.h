#pragma once
#include "pgpch.h"

#include "pgBuffer.h"

namespace Pagoda::Mirage {
    class IndexBuffer : public Buffer<unsigned int*> {
    public:
        static IndexBuffer* Create(unsigned int* data, const unsigned int size);

        virtual ~IndexBuffer() {}

        IndexBuffer(unsigned int* data, const unsigned int size) : m_Count(size / sizeof(unsigned int)), Buffer(data, size) {}

        inline unsigned int GetCount() {
            return this->m_Count;
        }

    private:
        unsigned int m_Count;
    };
}