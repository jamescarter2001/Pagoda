#include "pgpch.h"
#include "pg_d3d11_window.h"

namespace Pagoda::Mirage {
    D3D11Window::D3D11Window(const WindowProps& props) : Window(props) {
        this->m_Window = nullptr;

        this->m_DevicePtr = nullptr;
        this->m_DeviceContextPtr = nullptr;
        this->m_SwapChainPtr = nullptr;
        this->m_RenderTargetViewPtr = nullptr;

        this->m_SwapChainDesc = {0};

        Init();
    }

    D3D11Window::~D3D11Window() {
        this->m_DevicePtr->Release();
        this->m_DeviceContextPtr->Release();
        this->m_SwapChainPtr->Release();
        this->m_RenderTargetViewPtr->Release();
    }

    Window* Window::Create(const WindowProps& props) {
        return new D3D11Window(props);
    }

    LRESULT CALLBACK D3D11Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        // sort through and find what code to run for the message given
        switch (message) {
                // this message is read when the window is closed
            case WM_DESTROY: {
                // close the application entirely
                PostQuitMessage(0);
                return 0;
            } break;
        }

        // Handle any messages the switch statement didn't
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    void D3D11Window::Init() {
        PG_CORE_DEBUG("Using graphics API: Direct3D11");

        HINSTANCE hInstance = GetModuleHandle(NULL);

        // the handle for the window, filled by a function
        HWND hWnd;
        // this struct holds information for the window class
        WNDCLASSEX wc;

        // clear out the window class for use, initializing all values in the struct to 0
        ZeroMemory(&wc, sizeof(WNDCLASSEX));

        // fill in the struct with the needed information
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = Pagoda::Mirage::D3D11Window::WindowProc;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
        wc.lpszClassName = L"WindowClass1";

        // register the window class
        RegisterClassEx(&wc);

        // create the window and use the result as the handle
        hWnd = CreateWindowEx(NULL,
                              L"WindowClass1",                        // name of the window class
                              STR_TO_WSTR(this->GetTitle()).c_str(),  // title of the window
                              WS_OVERLAPPEDWINDOW,                    // window style
                              300,                                    // x-position of the window
                              300,                                    // y-position of the window
                              this->GetWidth(),                       // width of the window
                              this->GetHeight(),                      // height of the window
                              NULL,                                   // we have no parent window, NULL
                              NULL,                                   // we aren't using menus, NULL
                              hInstance,                              // application handle
                              NULL);                                  // used with multiple windows, NULL

        // display the window on the screen
        ShowWindow(hWnd, SW_SHOW);

        this->Direct3D11Init(hWnd);
    }

    void D3D11Window::Direct3D11Init(HWND hWnd) {
        // Numerator and denominator for drawing as fast as possible.

        m_SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        m_SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

        m_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

        // Default values here as no multi sample anti aliasing is in use.

        m_SwapChainDesc.SampleDesc.Count = 1;
        m_SwapChainDesc.SampleDesc.Quality = 0;

        m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        m_SwapChainDesc.BufferCount = 1;
        m_SwapChainDesc.OutputWindow = hWnd;
        m_SwapChainDesc.Windowed = true;

        D3D_FEATURE_LEVEL featureLevel;
        UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;

        #ifdef PG_DEBUG
        flags |= D3D11_CREATE_DEVICE_DEBUG;
        #endif

        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            NULL,                       // Pointer to the video adapter to use, passing null selects the default
            D3D_DRIVER_TYPE_HARDWARE,   // Represents the driver type to create
            NULL,                       // Handle to a DLL that implements a software rasteriser, must not be NULL if previous arg is D3D_DRIVER_TYPE_SOFTWARE
            flags,                      // Runtime layers to enable, values can be OR'd together
            NULL,                       // Determines the order of feature levels to create
            0,                          // The number of elements in the feature level array supplied above
            D3D11_SDK_VERSION,          // The SDK version to use
            &m_SwapChainDesc,             // Populate the pointers
            &m_SwapChainPtr,
            &m_DevicePtr,
            &featureLevel,
            &m_DeviceContextPtr
        );

        PG_CORE_DEBUG("D3D11CreateDeviceAndSwapChain HRESULT: {}", hr == S_OK ? "S_OK" : std::to_string(hr));

        PG_CORE_ASSERT_CRITICAL(hr == S_OK, "Direct3D11 failed to initialize");

        ID3D11Texture2D* framebuffer = NULL;
        hr = m_SwapChainPtr->GetBuffer(
            0,
            __uuidof(ID3D11Texture2D),
            (void**)&framebuffer);

        if (framebuffer != NULL) {
            hr = m_DevicePtr->CreateRenderTargetView(
                framebuffer, 0, &m_RenderTargetViewPtr);
            framebuffer->Release();
        } else {
            PG_CORE_WARNING("Unable to fetch framebuffer from swap chain pointer");
        }

        // Init context

        D3D11Context::Init(m_DevicePtr, m_DeviceContextPtr, m_SwapChainPtr, m_RenderTargetViewPtr);
    }

    void D3D11Window::BeforeUpdate() {
        float backgroundColour[4] = {
            0x64 / 255.0f, 0x95 / 255.0f, 0xED / 255.0f, 1.0f};
        this->m_DeviceContextPtr->ClearRenderTargetView(
            this->m_RenderTargetViewPtr, backgroundColour);

        RECT winRect;
        GetClientRect(this->m_Window, &winRect);
        D3D11_VIEWPORT viewport = {
            0.0f,
            0.0f,
            (FLOAT)(winRect.right - winRect.left),
            (FLOAT)(winRect.bottom - winRect.top),
            0.0f,
            1.0f};
        this->m_DeviceContextPtr->RSSetViewports(1, &viewport);

        this->m_DeviceContextPtr->OMSetRenderTargets(1, &this->m_RenderTargetViewPtr, NULL);
    }

    void D3D11Window::OnUpdate() {
        MSG msg;

        // Check to see if any messages are waiting in the queue
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            // translate keystroke messages into the right format
            TranslateMessage(&msg);

            // send the message to the WindowProc function
            DispatchMessage(&msg);

            // check to see if it's time to quit
            if (msg.message == WM_QUIT) {
                Base::WindowCloseEvent e = Base::WindowCloseEvent();
                this->m_WindowData.EventCallback(e);
            }
        } else {
            this->m_SwapChainPtr->Present(1, 0);
        }
    }
}