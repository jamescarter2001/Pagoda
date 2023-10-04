#pragma once
#include "pgpch.h"

namespace Pagoda::Mirage {
    template<typename T>
    class PAGODA_API Buffer {
    public:
        Buffer(T buffer[], int size) : Buffer(size) {
            memcpy(m_BufferData, buffer, size);
        }

        Buffer(int size) {
            this->m_BufferData = new T[size];
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

        inline T* GetBuffer() const {
            return m_BufferData;
        }

        virtual void Write(T buffer[]) const {
            memcpy(this->m_BufferData, buffer, this->m_BufferSize);
        }
    protected:
        T* m_BufferData;
        int m_BufferSize;
    };
}