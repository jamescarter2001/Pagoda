#pragma once
#include "pgpch.h"

#include "mirage/core/renderer/pg_renderer.h"

#include "mirage/platform/d3d12/context/pg_d3d12_context.h"

namespace Pagoda::Mirage {
    class D3D12Renderer : public Renderer {
    public:
        D3D12Renderer(D3D12Context context);
        virtual ~D3D12Renderer();

        virtual void Draw(const Model& model, const PipelineState* pipelineState) const override;

    private:
        D3D12Context m_context;
    };
}