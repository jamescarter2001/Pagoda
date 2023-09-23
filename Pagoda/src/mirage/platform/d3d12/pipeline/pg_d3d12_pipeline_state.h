#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "mirage/core/pipeline/pg_pipeline_state.h"
#include "mirage/core/buffer/pg_vertex_buffer_layout.h"

#include "mirage/platform/d3d/util/pg_d3d_util.h"

#include "mirage/platform/d3d12/context/pg_d3d12_context.h"
#include "mirage/platform/d3d12/shader/pg_d3d12_shader.h"

namespace Pagoda::Mirage {
    class D3D12PipelineState : public PipelineState {
    public:
        D3D12PipelineState(Shader* vertexShader, Shader* fragmentShader, VertexBufferLayout& vertexBufferLayout);
        virtual ~D3D12PipelineState();

        virtual void Bind() const override;
        virtual void Unbind() const override;

    private:
        ComPtr<ID3D12PipelineState> m_pipelineState;
    };
}