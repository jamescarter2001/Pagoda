#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"
#include "base/util/pg_strings.h"

#include "mirage/core/window/pg_window.h"

#include "mirage/platform/d3d11/context/pg_d3d11_context.h"

namespace Pagoda::Mirage {

    class PAGODA_API D3D11Window : public Window {
    public:
        D3D11Window(const WindowProps& props, std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> winProcCallback);
        virtual ~D3D11Window();

        virtual inline std::string GetApiName() override {
            return "Direct3D11";
        }

        void CreateRenderTarget();
        void CleanupRenderTarget();

        virtual std::shared_ptr<MirageFactory> Init() override;
        virtual void BeforeUpdate() override;
        virtual void OnUpdate() override;
    private:
        HWND m_Window;

        std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> m_winProcCallback = [](HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
            PG_CORE_WARNING("No WinProc callback set!");
            return false;
        };

        ID3D11Device* m_pDevice;
        ID3D11DeviceContext* m_pDeviceContext;
        IDXGISwapChain* m_pSwapChain;
        ID3D11RenderTargetView* m_pRenderTargetView;

        // this is the main message handler for the program
        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
        LRESULT CALLBACK InternalWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        std::shared_ptr<MirageFactory> Direct3D11Init();
    };
}