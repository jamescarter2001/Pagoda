#include "pgpch.h"
#include "pg_d3d11_window.h"

#include "mirage/platform/d3d11/factory/pg_d3d11_mirage_factory.h"

namespace Pagoda::Mirage {
    D3D11Window::D3D11Window(const WindowProps& props, std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> winProcCallback)
        : Window(props), m_winProcCallback(winProcCallback) {
        this->m_Window = nullptr;

        this->m_pDevice = nullptr;
        this->m_pDeviceContext = nullptr;
        this->m_pSwapChain = nullptr;
        this->m_pRenderTargetView = nullptr;

        m_mirageFactory = this->D3D11Window::Init();
    }

    D3D11Window::~D3D11Window() {
        this->m_pDevice->Release();
        this->m_pDeviceContext->Release();
        this->m_pSwapChain->Release();
        this->m_pRenderTargetView->Release();
    }

    LRESULT CALLBACK D3D11Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        D3D11Window* pThis = nullptr;

        if (message == WM_NCCREATE) {
            LPCREATESTRUCT pCreate = reinterpret_cast<LPCREATESTRUCT>(lParam);
            pThis = reinterpret_cast<D3D11Window*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
        } else {
            pThis = reinterpret_cast<D3D11Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        }

        if (pThis != nullptr) {
            return pThis->InternalWindowProc(hWnd, message, wParam, lParam);
        } else {
            PG_CORE_WARNING("Singleton pointer is null. Forwarding to static WindowProc");
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }

    LRESULT CALLBACK D3D11Window::InternalWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        if (m_winProcCallback(hWnd, message, wParam, lParam)) {
            return true;
        }

        // sort through and find what code to run for the message given
        switch (message) {
            // this message is read when the window is closed
            case WM_DESTROY: {
                // close the application entirely
                PostQuitMessage(0);
                return 0;
            }
            case WM_SIZE: {
                PG_CORE_INFO("Window Resize Event: {} {}", (UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
                if (m_pSwapChain != nullptr && wParam != SIZE_MINIMIZED) {
                    CleanupRenderTarget();
                    UINT test = (UINT)LOWORD(lParam);
                    HRESULT result = m_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_R8G8B8A8_UNORM, 0);
                    CreateRenderTarget();
                }
                return 0;
            }
                // default: PG_CORE_WARNING("Unhandled Windows message code: {}", message);
        }

        // Handle any messages the switch statement didn't
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    void D3D11Window::CreateRenderTarget() {
        ID3D11Texture2D* framebuffer = NULL;
        HRESULT hr = m_pSwapChain->GetBuffer(
            0,
            __uuidof(ID3D11Texture2D),
            (void**)&framebuffer);

        if (framebuffer != NULL) {
            hr = m_pDevice->CreateRenderTargetView(
                framebuffer, 0, &m_pRenderTargetView);
            framebuffer->Release();
        } else {
            PG_CORE_WARNING("Unable to fetch framebuffer from swap chain pointer");
        }
    }

    void D3D11Window::CleanupRenderTarget() {
        if (m_pRenderTargetView) {
            m_pRenderTargetView->Release();
            m_pRenderTargetView = nullptr;
        }
    }

    std::shared_ptr<MirageFactory> D3D11Window::Init() {
        PG_CORE_DEBUG("Using graphics API: Direct3D11");

        const HINSTANCE hInstance = GetModuleHandle(nullptr);

        // this struct holds information for the window class
        WNDCLASSEX wc;

        // clear out the window class for use, initializing all values in the struct to 0
        ZeroMemory(&wc, sizeof(WNDCLASSEX));

        // fill in the struct with the needed information
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = Pagoda::Mirage::D3D11Window::WindowProc;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
        wc.lpszClassName = L"WindowClass1";

        // register the window class
        RegisterClassEx(&wc);

        // create the window and use the result as the handle
        this->m_Window = CreateWindowEx(NULL,
                                        wc.lpszClassName,                                      // name of the window class
                                        Base::Strings::STR_TO_WSTR(this->GetTitle()).c_str(),  // title of the window
                                        WS_OVERLAPPEDWINDOW,                                   // window style
                                        300,                                                   // x-position of the window
                                        300,                                                   // y-position of the window
                                        this->GetWidth(),                                      // width of the window
                                        this->GetHeight(),                                     // height of the window
                                        NULL,                                                  // we have no parent window, NULL
                                        NULL,                                                  // we aren't using menus, NULL
                                        hInstance,                                             // application handle
                                        this);                                                 // used with multiple windows, NULL

        // display the window on the screen
        ShowWindow(this->m_Window, SW_SHOW);

        return this->Direct3D11Init();
    }

    std::shared_ptr<MirageFactory> D3D11Window::Direct3D11Init() {
        // Numerator and denominator for drawing as fast as possible.

        DXGI_SWAP_CHAIN_DESC swapChainDesc = {0};

        swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

        // Default values here as no multi sample anti aliasing is in use.

        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;

        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 2;
        swapChainDesc.OutputWindow = this->m_Window;
        swapChainDesc.Windowed = true;

        D3D_FEATURE_LEVEL featureLevel;
        UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;

#ifdef PG_DEBUG
        flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            NULL,                     // Pointer to the video adapter to use, passing null selects the default
            D3D_DRIVER_TYPE_HARDWARE, // Represents the driver type to create
            NULL,                     // Handle to a DLL that implements a software rasteriser, must not be NULL if previous arg is D3D_DRIVER_TYPE_SOFTWARE
            flags,                    // Runtime layers to enable, values can be OR'd together
            NULL,                     // Determines the order of feature levels to create
            0,                        // The number of elements in the feature level array supplied above
            D3D11_SDK_VERSION,        // The SDK version to use
            &swapChainDesc,           // Populate the pointers
            &m_pSwapChain,
            &m_pDevice,
            &featureLevel,
            &m_pDeviceContext
            );

        PG_CORE_DEBUG("D3D11CreateDeviceAndSwapChain HRESULT: {}", hr == S_OK ? "S_OK" : std::to_string(hr));

        PG_CORE_ASSERT_CRITICAL(hr == S_OK, "Direct3D11 failed to initialize");

        CreateRenderTarget();

        // Init context

        std::shared_ptr<D3D11Context> ctx = std::make_shared<D3D11Context>(m_Window, m_pDevice, m_pDeviceContext, m_pSwapChain, m_pRenderTargetView);
        
        return std::make_shared<D3D11MirageFactory>(m_windowData, ctx);
    }

    void D3D11Window::BeforeUpdate() {
        float backgroundColour[4] = {
            // 60.0f / 255.0f, 60.0f / 255.0f, 60.0f / 255.0f, 1.0f
            0.0f, 0.0f, 0.0f, 1.0f
        };
        this->m_pDeviceContext->ClearRenderTargetView(
            this->m_pRenderTargetView, backgroundColour);

        RECT winRect;
        GetClientRect(this->m_Window, &winRect);

        D3D11_VIEWPORT viewport = {
            0.0f,
            0.0f,
            (FLOAT)(winRect.right - winRect.left),
            (FLOAT)(winRect.bottom - winRect.top),
            0.0f,
            1.0f};
        this->m_pDeviceContext->RSSetViewports(1, &viewport);

        this->m_pDeviceContext->OMSetRenderTargets(1, &this->m_pRenderTargetView, NULL);
    }

    void D3D11Window::OnUpdate() {
        MSG msg;

        // Check to see if any messages are waiting in the queue
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            // translate keystroke messages into the right format
            TranslateMessage(&msg);

            // send the message to the WindowProc function
            DispatchMessage(&msg);

            // check to see if it's time to quit
            if (msg.message == WM_QUIT) {
                Base::WindowCloseEvent e = Base::WindowCloseEvent();
                this->m_windowData.EventCallback(e);
            }
        }

        this->m_pSwapChain->Present(1, 0);
    }
}
