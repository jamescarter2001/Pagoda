#include "pgpch.h"
#include "pg_d3d11_renderer.h"

namespace Pagoda::Mirage {
    D3D11Renderer::D3D11Renderer() {
        this->m_DeviceContext = D3D11Context().GetDeviceContextPtr();
    }

    D3D11Renderer::~D3D11Renderer() {

    }

    void D3D11Renderer::Draw(const Model& model, const PipelineState* pipelineState) const {
        Renderer::Draw(model, pipelineState);

        // this->m_DeviceContext->Draw(model.GetVertexCount(), 0);
        this->m_DeviceContext->DrawIndexed(model.GetVertexCount(), 0, 0);
    }
}