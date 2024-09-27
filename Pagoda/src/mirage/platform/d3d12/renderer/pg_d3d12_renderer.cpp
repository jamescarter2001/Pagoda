#include "pgpch.h"
#include "pg_d3d12_renderer.h"

namespace Pagoda::Mirage {
    D3D12Renderer::D3D12Renderer(std::shared_ptr<D3D12Context> context) : Renderer(), m_ctx(context) {

    }

    D3D12Renderer::~D3D12Renderer() {

    }

    ConstantBuffer<float>* D3D12Renderer::CreateMVP() {
        return new D3D12ConstantBuffer<float>(this->m_ctx, &this->m_mvpMatrix[0][0], sizeof(m_mvpMatrix), ConstantBufferType::CONSTANT_BUFFER_TYPE_MVP);
    }

    void D3D12Renderer::Draw(const Model& model, const PipelineState* pipelineState, const ConstantBuffer<float>* transform, bool project) {
        Renderer::Draw(model, pipelineState, transform, project);

        this->m_ctx->GetCommandList()->DrawIndexedInstanced(model.GetVertexCount(), 1, 0, 0, 0);
    }
}