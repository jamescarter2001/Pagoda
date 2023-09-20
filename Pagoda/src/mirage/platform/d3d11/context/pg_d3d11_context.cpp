#include "pgpch.h"
#include "pg_d3d11_context.h"

namespace Pagoda::Mirage {
    ID3D11Device* D3D11Context::ms_DevicePtr = NULL;
    ID3D11DeviceContext* D3D11Context::ms_DeviceContextPtr = NULL;
    IDXGISwapChain* D3D11Context::ms_SwapChainPtr = NULL;
    ID3D11RenderTargetView* D3D11Context::ms_RenderTargetViewPtr = NULL;

    void D3D11Context::Init(ID3D11Device* devicePtr, ID3D11DeviceContext* deviceContextPtr, IDXGISwapChain* swapChainPtr, ID3D11RenderTargetView* renderTargetViewPtr) {
        D3D11Context::ms_DevicePtr = devicePtr;
        D3D11Context::ms_DeviceContextPtr = deviceContextPtr;
        D3D11Context::ms_SwapChainPtr = swapChainPtr;
        D3D11Context::ms_RenderTargetViewPtr = renderTargetViewPtr;

        PG_CORE_INFO("Direct3D11 context ready");
    }
}