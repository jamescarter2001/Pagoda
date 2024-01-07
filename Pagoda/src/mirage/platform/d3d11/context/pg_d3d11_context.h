#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

namespace Pagoda::Mirage {
    class D3D11Context {
    public:
        D3D11Context(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, IDXGISwapChain* pSwapChain, ID3D11RenderTargetView* pRenderTargetView);
        virtual ~D3D11Context();

        inline ID3D11Device* GetDevice() const & {
            return this->m_device;
        }

        inline ID3D11DeviceContext* GetDeviceContext() const & {
            return this->m_deviceContext;
        }

        inline IDXGISwapChain* GetSwapChain() const & {
            return this->m_swapChain;
        }

        inline ID3D11RenderTargetView* GetRenderTargetView() const & {
            return this->m_renderTargetView;
        }

    private:
        ID3D11Device* m_device;
        ID3D11DeviceContext* m_deviceContext;
        IDXGISwapChain* m_swapChain;
        ID3D11RenderTargetView* m_renderTargetView;
    };
}
