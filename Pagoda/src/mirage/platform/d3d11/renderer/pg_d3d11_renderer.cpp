#include "pgpch.h"
#include "pg_d3d11_renderer.h"

namespace Pagoda::Mirage {
    D3D11Renderer::D3D11Renderer(D3D11Context context, WindowData* wd) : Renderer(wd) {
        this->m_DeviceContext = context.GetDeviceContextPtr();
    }

    D3D11Renderer::~D3D11Renderer() {

    }

    ConstantBuffer<float>* D3D11Renderer::CreateMVP() {
        // TODO: Add D3D11 constant buffer
        PG_CORE_WARNING("No implementation of D3D11ConstantBuffer!");
        return nullptr;
    }

    void D3D11Renderer::Draw(const Model& model, const PipelineState* pipelineState, const ConstantBuffer<float>* transform, bool project) {
        Renderer::Draw(model, pipelineState, transform, project);

        // this->m_DeviceContext->Draw(model.GetVertexCount(), 0);
        this->m_DeviceContext->DrawIndexed(model.GetVertexCount(), 0, 0);
    }
}