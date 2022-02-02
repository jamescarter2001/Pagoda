#pragma once
#include "pgpch.h"

#include "pgBuffer.h"

namespace Pagoda::Mirage {
	class VertexBuffer : public Buffer {
        VertexBuffer(const void* data);
        virtual ~VertexBuffer();
	};
}