#pragma once
#include "pgpch.h"

#include "pg_util.h"
#include "mirage/core/window/pg_window.h"

namespace Pagoda::Mirage {

    class D3D11Window : public Window {
    public:
        D3D11Window(const WindowProps& props);
        virtual ~D3D11Window() {}
        virtual void Init() override;
        virtual void OnUpdate() override;
    private:
        HWND m_Window;

        // this is the main message handler for the program
        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        void Direct3D11Init(HWND hWnd);
    };
}