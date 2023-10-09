#include "pgpch.h"
#include "pg_d3d11_renderer.h"

namespace Pagoda::Mirage {
    D3D11Renderer::D3D11Renderer(D3D11Context context, WindowData* wd) : Renderer(wd), m_context(context) {
    }

    D3D11Renderer::~D3D11Renderer() {

    }

    ConstantBuffer<float>* D3D11Renderer::CreateMVP() {
        return new D3D11ConstantBuffer<float>(this->m_context, &this->m_mvpMatrix[0][0], sizeof(m_mvpMatrix), ConstantBufferType::CONSTANT_BUFFER_TYPE_MVP);
    }

    void D3D11Renderer::Draw(const Model& model, const PipelineState* pipelineState, const ConstantBuffer<float>* transform, bool project) {
        Renderer::Draw(model, pipelineState, transform, project);

        // this->m_DeviceContext->Draw(model.GetVertexCount(), 0);
        this->m_context.GetDeviceContextPtr()->DrawIndexed(model.GetVertexCount(), 0, 0);
    }
}