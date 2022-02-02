#pragma once
#include "pgpch.h"

#include "pgBuffer.h"

namespace Pagoda::Mirage {
    class IndexBuffer : public Buffer {
        IndexBuffer(const unsigned int* data);
        virtual ~IndexBuffer();
    };
}