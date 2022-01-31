#pragma once
#include "pgpch.h"

#include "Mirage/Platform/GL/Buffer/pgGLBuffer.h"

namespace Pagoda::Mirage {
    class GLIndexBuffer : public GLBuffer<const unsigned int*> {
    };
}