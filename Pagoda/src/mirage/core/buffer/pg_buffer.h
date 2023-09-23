#pragma once
#include "pgpch.h"

namespace Pagoda::Mirage {
    template<typename T>
    class PAGODA_API Buffer {
    public:
        Buffer(T buffer[], int size) {
            this->m_BufferData = new T[size];
            memcpy(m_BufferData, buffer, size);

            this->m_BufferSize = size;
        }

        virtual ~Buffer() {
            delete this->m_BufferData;
        }

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        inline unsigned int GetSize() const {
            return m_BufferSize;
        }
    protected:
        T* m_BufferData;
        int m_BufferSize;
    };
}