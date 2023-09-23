#include "pgpch.h"
#include "pg_d3d12_renderer.h"

namespace Pagoda::Mirage {
    D3D12Renderer::D3D12Renderer() {
        this->m_commandList = D3D12Context().GetCommandList();
    }

    D3D12Renderer::~D3D12Renderer() {

    }

    void D3D12Renderer::Draw(const Model& model, const PipelineState* pipelineState) const {
        Renderer::Draw(model, pipelineState);

        // this->m_DeviceContext->Draw(model.GetVertexCount(), 0);
        this->m_commandList->DrawInstanced(model.GetVertexCount(), 1, 0, 0);
    }
}