#pragma once
#include "pgpch.h"

namespace Pagoda::Mirage {
    template<typename T>
    class PAGODA_API Buffer {
    public:
        Buffer(T* buffer, int count) {
            this->m_BufferData = buffer;
            this->m_Count = count;
        }

        virtual ~Buffer() {}
    protected:
        T* m_BufferData;
        int m_Count;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
    };
}