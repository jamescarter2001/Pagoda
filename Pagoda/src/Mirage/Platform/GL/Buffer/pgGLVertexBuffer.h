#pragma once
#include "pgpch.h"

#include <glad/glad.h>

#include "Mirage/MirageCore/Buffer/pgVertexBuffer.h"

namespace Pagoda::Mirage {
    class PAGODA_API GLVertexBuffer : public VertexBuffer {
    public:
        static VertexBuffer* Create(void* data, const unsigned int size);

        GLVertexBuffer(void* data, const unsigned int size);
        ~GLVertexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;
    };
}
