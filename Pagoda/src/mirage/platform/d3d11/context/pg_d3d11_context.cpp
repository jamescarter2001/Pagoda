#include "pgpch.h"
#include "pg_d3d11_context.h"

namespace Pagoda::Mirage {
    D3D11Context::D3D11Context(HWND& window, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, IDXGISwapChain* pSwapChain, ID3D11RenderTargetView* pRenderTargetView) : m_hWnd(window) {
        m_device = pDevice;
        m_deviceContext = pDeviceContext;
        m_swapChain = pSwapChain;
        m_renderTargetView = pRenderTargetView;

        PG_CORE_INFO("Direct3D11 context ready");
    }

    D3D11Context::~D3D11Context() {
    }
}
