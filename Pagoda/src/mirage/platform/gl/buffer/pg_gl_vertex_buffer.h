#pragma once
#include "pgpch.h"

#include <glad/glad.h>

#include "mirage/core/buffer/pg_vertex_buffer.h"

namespace Pagoda::Mirage {
    class PAGODA_API GLVertexBuffer : public VertexBuffer {
    public:
        GLVertexBuffer(void* data, const unsigned int size);
        ~GLVertexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;

    private:
        unsigned int m_RendererID;
    };
}
