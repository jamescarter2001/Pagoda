#pragma once
#include "pgpch.h"

namespace Pagoda::Mirage {
    template<typename T>
    class PAGODA_API Buffer {
    public:
        Buffer(T buffer[], int bufferCount) {
            this->m_BufferData = new T[bufferCount];
            memcpy(m_BufferData, buffer, bufferCount);

            this->m_BufferCount = bufferCount;
        }

        virtual ~Buffer() {
            delete this->m_BufferData;
        }

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        inline unsigned int GetBufferCount() const {
            return m_BufferCount;
        }
    protected:
        T* m_BufferData;
        int m_BufferCount;
    };
}