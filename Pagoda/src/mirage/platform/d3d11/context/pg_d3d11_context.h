#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

namespace Pagoda::Mirage {
    class D3D11Context {
    public:
        static void Init(ID3D11Device* devicePtr, ID3D11DeviceContext* deviceContextPtr, IDXGISwapChain* swapChainPtr, ID3D11RenderTargetView* renderTargetViewPtr);

        inline ID3D11Device* GetDevicePtr() const& {
            return ms_DevicePtr;
        }

        inline ID3D11DeviceContext* GetDeviceContextPtr() const& {
            return ms_DeviceContextPtr;
        }

        inline IDXGISwapChain* GetSwapChainPtr() const& {
            return ms_SwapChainPtr;
        }

        inline ID3D11RenderTargetView* GetRenderTargetViewPtr() const& {
            return ms_RenderTargetViewPtr;
        }
    private:
        static ID3D11Device* ms_DevicePtr;
        static ID3D11DeviceContext* ms_DeviceContextPtr;
        static IDXGISwapChain* ms_SwapChainPtr;
        static ID3D11RenderTargetView* ms_RenderTargetViewPtr;
    };
}