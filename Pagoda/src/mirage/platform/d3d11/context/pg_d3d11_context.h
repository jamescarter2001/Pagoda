#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

namespace Pagoda::Mirage {
    class D3D11Context {
    public:
        D3D11Context(ID3D11Device* devicePtr, ID3D11DeviceContext* deviceContextPtr, IDXGISwapChain* swapChainPtr, ID3D11RenderTargetView* renderTargetViewPtr);
        virtual ~D3D11Context() {}

        inline ID3D11Device* GetDevicePtr() const& {
            return m_DevicePtr;
        }

        inline ID3D11DeviceContext* GetDeviceContextPtr() const& {
            return m_DeviceContextPtr;
        }

        inline IDXGISwapChain* GetSwapChainPtr() const& {
            return m_SwapChainPtr;
        }

        inline ID3D11RenderTargetView* GetRenderTargetViewPtr() const& {
            return m_RenderTargetViewPtr;
        }
    private:
        static ID3D11Device* m_DevicePtr;
        static ID3D11DeviceContext* m_DeviceContextPtr;
        static IDXGISwapChain* m_SwapChainPtr;
        static ID3D11RenderTargetView* m_RenderTargetViewPtr;
    };
}