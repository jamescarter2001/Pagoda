#include "pgpch.h"
#include "pgGLVertexBuffer.h"

namespace Pagoda::Mirage {
    GLVertexBuffer::GLVertexBuffer(const void* data, const unsigned int size) : GLBuffer(data, size, GL_ARRAY_BUFFER) {}
}