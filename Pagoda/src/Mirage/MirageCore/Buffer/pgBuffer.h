#pragma once
#include "pgpch.h"

namespace Pagoda::Mirage {
    template<typename T>
    class PAGODA_API Buffer {
    public:
        virtual ~Buffer() {}

        inline T GetBufferData() {
            return m_BufferData;
        }

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
    protected:
        unsigned int m_RendererID;

        T m_BufferData;
        unsigned int m_BufferSize;

       Buffer(T data, const unsigned int size) {
            m_RendererID = 0;
            m_BufferData = data;
            m_BufferSize = size;
        }
    };
}