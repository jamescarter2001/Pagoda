#include "pgpch.h"
#include "pg_renderer.h"

namespace Pagoda::Mirage {
    Renderer::Renderer() {}

    Renderer::~Renderer() {}

    void Renderer::Draw(const Model& model, const PipelineState* pipelineState) const {
        pipelineState->Bind();
        model.Bind();
    }
}