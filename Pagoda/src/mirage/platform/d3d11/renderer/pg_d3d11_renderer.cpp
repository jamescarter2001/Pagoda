#include "pgpch.h"
#include "pg_d3d11_renderer.h"

namespace Pagoda::Mirage {
    D3D11Renderer::D3D11Renderer() {
        this->m_DeviceContext = D3D11Context().GetDeviceContextPtr();
    }

    D3D11Renderer::~D3D11Renderer() {

    }

    void D3D11Renderer::Draw(Model& model, Shader& shader) {
        Renderer::Draw(model, shader);

        this->m_DeviceContext->Draw(model.GetVertexCount(), 0);
    }
}