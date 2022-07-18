#pragma once
#include <glad/glad.h>

#include "Mirage/MirageCore/Renderer/pgRenderer.h"
#include "Mirage/MirageCore/Shader/pgShader.h"

#include "Mirage/Platform/GL/Buffer/pgGLVertexArray.h"

namespace Pagoda::Mirage {
    class GLRenderer : public Renderer {
    public:
        GLRenderer(Model& model, Shader& shader);
        virtual ~GLRenderer();

        virtual void Draw() const override;

    private:
        Shader& m_Shader;

        GLVertexArray* m_Vao;
    };
}