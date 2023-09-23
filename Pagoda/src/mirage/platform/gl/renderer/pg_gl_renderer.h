#pragma once
#include <glad/glad.h>

#include "mirage/core/renderer/pg_renderer.h"
#include "mirage/core/shader/pg_shader.h"

#include "mirage/platform/gl/buffer/pg_gl_vertex_array.h"

namespace Pagoda::Mirage {
    class GLRenderer : public Renderer {
    public:
        GLRenderer(Model& model, Shader& shader);
        virtual ~GLRenderer();

        virtual void Draw(const Model& model, const ShaderData& shader) const override;

    private:
        GLVertexArray* m_Vao;
    };
}