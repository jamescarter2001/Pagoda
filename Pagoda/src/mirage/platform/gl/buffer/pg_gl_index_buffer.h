#pragma once
#include "pgpch.h"

#include <glad/glad.h>

#include "mirage/core/buffer/pg_index_buffer.h"

namespace Pagoda::Mirage {
    class PAGODA_API GLIndexBuffer : public IndexBuffer {
    public:
        GLIndexBuffer(int data[], int bufferCount);
        ~GLIndexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;

    private:
        unsigned int m_RendererID;
    };
}