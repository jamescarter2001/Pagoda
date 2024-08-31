#pragma once
#include "pgpch.h"

#include "mirage/core/renderer/pg_renderer.h"
#include "mirage/core/window/pg_window_data.h"
#include "mirage/core/pipeline/pg_pipeline_state.h"

#include "mirage/platform/d3d11/context/pg_d3d11_context.h"
#include "mirage/platform/d3d11/buffer/pg_d3d11_constant_buffer.h"

namespace Pagoda::Mirage {
    class D3D11Renderer : public Renderer {
    public:
        D3D11Renderer(std::shared_ptr<D3D11Context> ctx);
        virtual ~D3D11Renderer();

        virtual void Draw(const Model& model, const PipelineState* pipelineState, const ConstantBuffer<float>* transform, bool project) override;
        virtual ConstantBuffer<float>* CreateMVP() override;

    private:
        std::shared_ptr<D3D11Context> m_context;
    };
}