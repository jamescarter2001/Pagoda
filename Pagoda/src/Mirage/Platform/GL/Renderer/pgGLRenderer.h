#pragma once
#include <glad/glad.h>

#include "Mirage/MirageCore/Renderer/pgRenderer.h"

#include "Mirage/Platform/GL/Buffer/pgGLVertexArray.h"

namespace Pagoda::Mirage {
    class GLRenderer : public Renderer {
    public:
        GLRenderer();
        ~GLRenderer();

        virtual void Clear() const override;
        virtual void Draw(Model& model) const override;
    };
}