#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

namespace Pagoda::Mirage {
    class D3D11Context {
    public:
        D3D11Context(ID3D11Device* devicePtr, ID3D11DeviceContext* deviceContextPtr, IDXGISwapChain* swapChainPtr, ID3D11RenderTargetView* renderTargetViewPtr);
        virtual ~D3D11Context();

        inline ID3D11Device* GetDevicePtr() const& {
            return this->m_device;
        }

        inline ID3D11DeviceContext* GetDeviceContextPtr() const& {
            return this->m_deviceContext;
        }

        inline IDXGISwapChain* GetSwapChainPtr() const& {
            return this->m_swapChain;
        }

        inline ID3D11RenderTargetView* GetRenderTargetViewPtr() const& {
            return this->m_renderTargetView;
        }
    private:
        ID3D11Device* m_device;
        ID3D11DeviceContext* m_deviceContext;
        IDXGISwapChain* m_swapChain;
        ID3D11RenderTargetView* m_renderTargetView;
    };
}