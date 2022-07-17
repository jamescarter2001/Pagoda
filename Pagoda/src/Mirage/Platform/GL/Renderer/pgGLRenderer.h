#pragma once
#include <glad/glad.h>

#include "Mirage/MirageCore/Renderer/pgRenderer.h"

#include "Mirage/Platform/GL/Buffer/pgGLVertexArray.h"
#include "Mirage/Platform/GL/Shader/Shader.h"

namespace Pagoda::Mirage {
    class GLRenderer : public Renderer {
    public:
        GLRenderer(Model& model, GLShader& shader);
        ~GLRenderer();

        virtual void Clear() const override;
        virtual void Draw() const override;

    private:
        GLShader& m_Shader;

        GLVertexArray* m_Vao;
    };
}