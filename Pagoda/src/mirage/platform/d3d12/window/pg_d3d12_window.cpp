#include "pgpch.h"
#include "pg_d3d12_window.h"

namespace Pagoda::Mirage {
    D3D12Window::D3D12Window(const WindowProps& props) : Window(props) {
        Init();
    }

    D3D12Window::~D3D12Window() {
    }

    Window* Window::Create(const WindowProps& props) {
        return new D3D12Window(props);
    }

    LRESULT CALLBACK D3D12Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
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

    void D3D12Window::Init() {
        PG_CORE_DEBUG("Using graphics API: Direct3D12");

        HINSTANCE hInstance = GetModuleHandle(NULL);

        // this struct holds information for the window class
        WNDCLASSEX wc;

        // clear out the window class for use, initializing all values in the struct to 0
        ZeroMemory(&wc, sizeof(WNDCLASSEX));

        // fill in the struct with the needed information
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = Pagoda::Mirage::D3D12Window::WindowProc;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
        wc.lpszClassName = L"WindowClass1";

        // register the window class
        RegisterClassEx(&wc);

        // create the window and use the result as the handle
        this->m_Window = CreateWindowEx(NULL,
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
        ShowWindow(this->m_Window, SW_SHOW);

        this->Direct3D12Init();
    }

    void D3D12Window::Direct3D12Init() {
        #ifdef PG_DEBUG

        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
            debugController->EnableDebugLayer();
        }

        #endif

        ComPtr<IDXGIFactory4> factory;
        LogOnError(CreateDXGIFactory1(IID_PPV_ARGS(&factory)), "Failed to create IDXGIFactory4");

        ComPtr<IDXGIAdapter1> hardwareAdapter;
        GetHardwareAdapter(factory.Get(), &hardwareAdapter);

        LogOnError(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_device)), "Failed to create ID3D12Device");

        // Describe and create the command queue.
        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

        LogOnError(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)), "Failed to create Command Queue");

        // Describe and create the swap chain.
        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        swapChainDesc.BufferCount = FrameCount;
        swapChainDesc.BufferDesc.Width = this->GetWidth();
        swapChainDesc.BufferDesc.Height = this->GetHeight();
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.OutputWindow = this->m_Window;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.Windowed = TRUE;

        ComPtr<IDXGISwapChain> swapChain;
        LogOnError(factory->CreateSwapChain(
            m_commandQueue.Get(),  // Swap chain needs the queue so that it can force a flush on it.
            &swapChainDesc,
            &swapChain),
            "Failed to create Swap Chain");

        LogOnError(swapChain.As(&m_swapChain), "Failed to assign Swap Chain");

        // This sample does not support fullscreen transitions.
        LogOnError(factory->MakeWindowAssociation(this->m_Window, DXGI_MWA_NO_ALT_ENTER), "Failed to make window association");

        m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

        // Create descriptor heaps.
        {
            // Describe and create a render target view (RTV) descriptor heap.
            D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
            rtvHeapDesc.NumDescriptors = FrameCount;
            rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
            rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
            LogOnError(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)), "Failed to create Descriptor Heap");

            m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        }

        // Create frame resources.
        {
            CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

            // Create a RTV for each frame.
            for (UINT n = 0; n < FrameCount; n++) {
                LogOnError(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])), "Failed to create Render Target View");
                m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
                rtvHandle.Offset(1, m_rtvDescriptorSize);
            }
        }

        LogOnError(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)), "Failed to create Command Allocator");

        // Init context

        // D3D12Context::Init(m_devicePtr, m_deviceContextPtr, m_swapChainPtr, m_renderTargetViewPtr);
    }

    void D3D12Window::GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter) {
        *ppAdapter = nullptr;
        for (UINT adapterIndex = 0;; ++adapterIndex) {
            IDXGIAdapter1* pAdapter = nullptr;
            if (DXGI_ERROR_NOT_FOUND == pFactory->EnumAdapters1(adapterIndex, &pAdapter)) {
                // No more adapters to enumerate.
                break;
            }

            // Check to see if the adapter supports Direct3D 12, but don't create the
            // actual device yet.
            if (SUCCEEDED(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), nullptr))) {
                PG_CORE_INFO("Found Direct3D12 compatible device");
                *ppAdapter = pAdapter;
                return;
            }
            pAdapter->Release();
        }
    }

    void D3D12Window::LogOnError(HRESULT hr, char err[] = "Failed to initialise Direct3D12 context") {
        if (hr != S_OK) {
            PG_CORE_CRITICAL(err);
        }
    }

    void D3D12Window::BeforeUpdate() {
        /* float backgroundColour[4] = {
            // 60.0f / 255.0f, 60.0f / 255.0f, 60.0f / 255.0f, 1.0f
            0.0f, 0.0f, 0.0f, 1.0f};
        this->m_deviceContextPtr->ClearRenderTargetView(
            this->m_renderTargetViewPtr, backgroundColour);

        RECT winRect;
        GetClientRect(this->m_Window, &winRect);

        D3D11_VIEWPORT viewport = {
            0.0f,
            0.0f,
            (FLOAT)(winRect.right - winRect.left),
            (FLOAT)(winRect.bottom - winRect.top),
            0.0f,
            1.0f};
        this->m_deviceContextPtr->RSSetViewports(1, &viewport);

        this->m_deviceContextPtr->OMSetRenderTargets(1, &this->m_renderTargetViewPtr, NULL);*/
    }

    void D3D12Window::OnUpdate() {
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
        }

        //this->m_swapChainPtr->Present(1, 0);
    }
}