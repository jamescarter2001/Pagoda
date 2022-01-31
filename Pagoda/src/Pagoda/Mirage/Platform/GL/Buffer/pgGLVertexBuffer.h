#pragma once
#include "pgpch.h"

#include <glad/glad.h>

#include "pgGLBuffer.h"

namespace Pagoda::Mirage {
    class GLVertexBuffer : public GLBuffer<const void*> {
    public:
        GLVertexBuffer(const void* data, const unsigned int size);
    };
}