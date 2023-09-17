#include "pgpch.h"
#include "pg_renderer.h"

namespace Pagoda::Mirage {
    void Renderer::Draw(Model& model, Shader& shader) {
        model.Bind();
        shader.Bind();
    }
}