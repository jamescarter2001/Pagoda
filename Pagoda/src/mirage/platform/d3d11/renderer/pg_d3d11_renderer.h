#pragma once
#include "pgpch.h"

#include "mirage/core/renderer/pg_renderer.h"
#include "mirage/core/pipeline/pg_pipeline_state.h"

#include "mirage/platform/d3d11/context/pg_d3d11_context.h"

namespace Pagoda::Mirage {
    class D3D11Renderer : public Renderer {
    public:
        D3D11Renderer();
        virtual ~D3D11Renderer();

        virtual void Draw(const Model& model, const PipelineState* pipelineState) const override;

    private:
        ID3D11DeviceContext* m_DeviceContext;
    };
}