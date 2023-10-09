#include "pgpch.h"
#include "pg_d3d11_context.h"

namespace Pagoda::Mirage {
    D3D11Context::D3D11Context(ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain, ID3D11RenderTargetView* renderTargetView) {
        this->m_device = device;
        this->m_deviceContext = deviceContext;
        this->m_swapChain = swapChain;
        this->m_renderTargetView = renderTargetView;

        PG_CORE_INFO("Direct3D11 context ready");
    }

    D3D11Context::~D3D11Context() {

    }
}