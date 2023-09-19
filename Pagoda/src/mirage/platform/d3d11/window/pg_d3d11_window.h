#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"
#include "base/util/pg_util.h"

#include "mirage/core/window/pg_window.h"

#include "mirage/platform/d3d11/context/pg_d3d11_context.h"

namespace Pagoda::Mirage {

    class PAGODA_API D3D11Window : public Window {
    public:
        D3D11Window(const WindowProps& props);
        virtual ~D3D11Window();
        virtual void Init() override;
        virtual void BeforeUpdate() override;
        virtual void OnUpdate() override;
    private:
        HWND m_Window;

        ID3D11Device* m_DevicePtr;
        ID3D11DeviceContext* m_DeviceContextPtr;
        IDXGISwapChain* m_SwapChainPtr;
        ID3D11RenderTargetView* m_RenderTargetViewPtr;

        DXGI_SWAP_CHAIN_DESC m_SwapChainDesc = {0};

        // this is the main message handler for the program
        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        void Direct3D11Init();
    };
}