#pragma once
#include "pgpch.h"

#include <glad/glad.h>

#include "Mirage/MirageCore/Buffer/pgIndexBuffer.h"

namespace Pagoda::Mirage {
    class GLIndexBuffer : public IndexBuffer {
    public:
        static IndexBuffer* Create(unsigned int* data, const unsigned int size);

        GLIndexBuffer(unsigned int* data, const unsigned int size);
        ~GLIndexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;
    };
}