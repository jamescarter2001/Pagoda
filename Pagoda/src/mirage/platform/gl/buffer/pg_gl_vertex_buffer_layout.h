#pragma once

#include "pgpch.h"

#include <glad/glad.h>

#include "mirage/core/buffer/pg_vertex_buffer_layout.h"

namespace Pagoda::Mirage {
    class PAGODA_API GLVertexBufferLayout : public VertexBufferLayout {
    public:
        virtual ~GLVertexBufferLayout();

        virtual void PushFloat(unsigned int count) override;
        
        virtual void PushInt(unsigned int count) override;
        
        virtual void PushChar(unsigned int count) override;
    };
}
