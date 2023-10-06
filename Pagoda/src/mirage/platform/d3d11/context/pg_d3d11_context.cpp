#include "pgpch.h"
#include "pg_d3d11_context.h"

namespace Pagoda::Mirage {
    D3D11Context::D3D11Context(ID3D11Device* devicePtr, ID3D11DeviceContext* deviceContextPtr, IDXGISwapChain* swapChainPtr, ID3D11RenderTargetView* renderTargetViewPtr) {
        m_DevicePtr = devicePtr;
        m_DeviceContextPtr = deviceContextPtr;
        m_SwapChainPtr = swapChainPtr;
        m_RenderTargetViewPtr = renderTargetViewPtr;

        PG_CORE_INFO("Direct3D11 context ready");
    }
}