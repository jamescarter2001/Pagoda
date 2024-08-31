#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"
#include "base/util/pg_strings.h"

#include "mirage/pg_mirage_factory.h"
#include "mirage/core/window/pg_window.h"

#include "mirage/pg_mirage_factory.h"
#include "mirage/platform/d3d11/context/pg_d3d11_context.h"

namespace Pagoda::Mirage {

    class PAGODA_API D3D11Window : public Window {
    public:
        D3D11Window(const WindowProps& props);
        virtual ~D3D11Window();
        virtual std::shared_ptr<MirageFactory> Init() override;
        virtual void BeforeUpdate() override;
        virtual void OnUpdate() override;
    private:
        HWND m_Window;

        ID3D11Device* m_devicePtr;
        ID3D11DeviceContext* m_deviceContextPtr;
        IDXGISwapChain* m_swapChainPtr;
        ID3D11RenderTargetView* m_renderTargetViewPtr;

        DXGI_SWAP_CHAIN_DESC m_SwapChainDesc = {0};

        // this is the main message handler for the program
        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        std::shared_ptr<MirageFactory> Direct3D11Init();
    };
}