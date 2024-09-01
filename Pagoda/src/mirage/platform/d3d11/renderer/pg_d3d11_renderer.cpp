#include "pgpch.h"
#include "pg_d3d11_renderer.h"

namespace Pagoda::Mirage {
    D3D11Renderer::D3D11Renderer(std::shared_ptr<D3D11Context> ctx) : Renderer(), m_context(ctx) {
    }

    D3D11Renderer::~D3D11Renderer() {

    }

    ConstantBuffer<float>* D3D11Renderer::CreateMVP() {
        return new D3D11ConstantBuffer<float>(m_context, &this->m_mvpMatrix[0][0], sizeof(m_mvpMatrix), ConstantBufferType::CONSTANT_BUFFER_TYPE_MVP);
    }

    void D3D11Renderer::Draw(const Model& model, const PipelineState* pipelineState, const ConstantBuffer<float>* transform, bool project) {
        Renderer::Draw(model, pipelineState, transform, project);

        // m_context->GetDeviceContext()->Draw(model.GetVertexCount(), 0);
        m_context->GetDeviceContext()->DrawIndexed(model.GetVertexCount(), 0, 0);
    }
}