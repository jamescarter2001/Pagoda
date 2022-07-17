#pragma once

#include <glad/glad.h>

#include "Mirage/MirageCore/Buffer/pgVertexBuffer.h"
#include "Mirage/MirageCore/Buffer/pgVertexBufferLayout.h"

namespace Pagoda::Mirage {
    class GLVertexArray {
    private:
        unsigned int m_RendererID;

    public:
        GLVertexArray();
        ~GLVertexArray();

        void Bind() const;
        void Unbind() const;

        void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    };
}