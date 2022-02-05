#pragma once

#include "pgpch.h"

#include <glad/glad.h>

#include "Mirage/MirageCore/Buffer/pgVertexBufferLayout.h"

namespace Pagoda::Mirage {
    class GLVertexBufferLayout : public VertexBufferLayout {
    public:
        virtual ~GLVertexBufferLayout();

        virtual void PushFloat(unsigned int count) override;
        
        virtual void PushInt(unsigned int count) override;
        
        virtual void PushChar(unsigned int count) override;
    };
}
