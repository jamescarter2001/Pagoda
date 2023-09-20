#include "pgpch.h"
#include "pg_renderer.h"

namespace Pagoda::Mirage {
    Renderer::Renderer() {}

    Renderer::~Renderer() {}

    void Renderer::Draw(const Model& model, const ShaderData& shader) const {
        model.Bind();
        shader.vertexShader->Bind();
        shader.fragmentShader->Bind();
    }
}