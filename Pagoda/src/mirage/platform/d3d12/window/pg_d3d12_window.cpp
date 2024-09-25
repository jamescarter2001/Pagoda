#include "pgpch.h"
#include "pg_d3d12_window.h"

#include "imgui.h"

#include "mirage/platform/d3d12/factory/pg_d3d12_mirage_factory.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Pagoda::Mirage {
    D3D12Window::D3D12Window(const WindowProps& props, std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> winProcCallback)
        : Window(props), m_winProcCallback(winProcCallback) {
        m_mirageFactory = this->D3D12Window::Init();
    }

    D3D12Window::~D3D12Window() {
    }

    LRESULT CALLBACK D3D12Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        D3D12Window* pThis = nullptr;

        if (message == WM_NCCREATE) {
            LPCREATESTRUCT pCreate = reinterpret_cast<LPCREATESTRUCT>(lParam);
            pThis = reinterpret_cast<D3D12Window*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
        }
        else {
            pThis = reinterpret_cast<D3D12Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        }

        if (pThis != nullptr) {
            return pThis->InternalWindowProc(hWnd, message, wParam, lParam);
        }
        else {
            PG_CORE_WARNING("Singleton pointer is null. Forwarding to static WindowProc");
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }

    LRESULT CALLBACK D3D12Window::InternalWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

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
                if (m_device != nullptr && wParam != SIZE_MINIMIZED) {
                    WaitForPreviousFrame();
                    CleanupRenderTarget();
                    UINT test = (UINT)LOWORD(lParam);
                    HRESULT result = m_swapChain->ResizeBuffers(FrameCount, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_R8G8B8A8_UNORM, 0);
                    CreateRenderTarget();
                }
                return 0;
            }
            // default: PG_CORE_WARNING("Unhandled Windows message code: {}", message);
        }

        // Handle any messages the switch statement didn't
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    std::shared_ptr<MirageFactory> D3D12Window::Init() {
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
                                        wc.lpszClassName,                                     // name of the window class
                                        Base::Strings::STR_TO_WSTR(this->GetTitle()).c_str(), // title of the window
                                        WS_OVERLAPPEDWINDOW,                                  // window style
                                        300,                                                  // x-position of the window
                                        300,                                                  // y-position of the window
                                        this->GetWidth(),                                     // width of the window
                                        this->GetHeight(),                                    // height of the window
                                        NULL,                                                 // we have no parent window, NULL
                                        NULL,                                                 // we aren't using menus, NULL
                                        hInstance,                                            // application handle
                                        this);                                                // used with multiple windows, NULL

        // display the window on the screen
        ShowWindow(this->m_Window, SW_SHOWDEFAULT);

        return this->Direct3D12Init();
    }

    std::shared_ptr<MirageFactory> D3D12Window::Direct3D12Init() {
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

        LogOnError(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&this->m_device)), "Failed to create ID3D12Device");

        // Describe and create the command queue.
        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

        LogOnError(this->m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&this->m_commandQueue)), "Failed to create Command Queue");

        // Describe and create the swap chain.
        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        swapChainDesc.BufferCount = FrameCount;
        swapChainDesc.BufferDesc.Width = 0; // Take from window.
        swapChainDesc.BufferDesc.Height = 0; // Take from window.
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.OutputWindow = this->m_Window;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.Windowed = TRUE;

        ComPtr<IDXGISwapChain> swapChain;
        LogOnError(factory->CreateSwapChain(
                       this->m_commandQueue.Get(), // Swap chain needs the queue so that it can force a flush on it.
                       &swapChainDesc,
                       &swapChain),
                   "Failed to create Swap Chain");

        LogOnError(swapChain.As(&this->m_swapChain), "Failed to assign Swap Chain");

        // This sample does not support fullscreen transitions.
        LogOnError(factory->MakeWindowAssociation(this->m_Window, DXGI_MWA_NO_ALT_ENTER), "Failed to make window association");

        this->m_frameIndex = this->m_swapChain->GetCurrentBackBufferIndex();

        // Create descriptor heaps.
        {
            // Describe and create a render target view (RTV) descriptor heap.
            D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
            rtvHeapDesc.NumDescriptors = FrameCount;
            rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
            rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
            LogOnError(this->m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&this->m_rtvHeap)), "Failed to create RTV Descriptor Heap");

            this->m_rtvDescriptorSize = this->m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

            D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
            srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            srvHeapDesc.NumDescriptors = 1;
            srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
            LogOnError(this->m_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&this->m_srvHeap)), "Failed to create SRV Descriptor Heap");
        }

        // Create frame resources.
        CreateRenderTarget();

        LogOnError(this->m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&this->m_commandAllocator)), "Failed to create Command Allocator");

        // Create an empty root signature.
        {
            CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
            D3D12_ROOT_PARAMETER params[2];

            // MVP
            params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
            params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
            params[0].Descriptor.ShaderRegister = 0;
            params[0].Descriptor.RegisterSpace = 0;

            // Transform
            params[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
            params[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
            params[1].Descriptor.ShaderRegister = 1;
            params[1].Descriptor.RegisterSpace = 0;

            rootSignatureDesc.Init(2, &params[0], 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

            ComPtr<ID3DBlob> signature;
            ComPtr<ID3DBlob> error;
            LogOnError(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error), "Failed to serialize Root Signature");
            LogOnError(this->m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&this->m_rootSignature)), "Failed to create Root Signature");
        }

        // Create synchronization objects and wait until assets have been uploaded to the GPU.
        {
            LogOnError(this->m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&this->m_fence)), "Failed to create Fence");
            this->m_fenceValue = 1;

            // Create an event handle to use for frame synchronization.
            this->m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
            if (this->m_fenceEvent == nullptr) {
                LogOnError(HRESULT_FROM_WIN32(GetLastError()));
            }
        }

        LogOnError(this->m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, this->m_commandAllocator.Get(), NULL, IID_PPV_ARGS(&this->m_commandList)), "Failed to create Command List");
        LogOnError(this->m_commandList->Close(), "Failed to close initial Command List");

        // Init context

        std::shared_ptr<D3D12Context> ctx = std::make_shared<D3D12Context>(m_Window, this->m_swapChain, this->m_device, this->m_renderTargets, this->m_commandAllocator, this->m_commandQueue, this->m_rootSignature, this->m_rtvHeap, this->m_srvHeap, this->m_commandList);
        return std::make_shared<D3D12MirageFactory>(m_windowData, ctx);
    }

    void D3D12Window::WaitForPreviousFrame() {
        // WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
        // This is code implemented as such for simplicity. More advanced samples
        // illustrate how to use fences for efficient resource usage.

        // Signal and increment the fence value.
        const UINT64 fence = this->m_fenceValue;
        this->m_fenceValue++;
        LogOnError(this->m_commandQueue->Signal(this->m_fence.Get(), fence), "Failed to send signal");

        // Wait until the previous frame is finished.
        if (this->m_fence->GetCompletedValue() < fence) {
            LogOnError(this->m_fence->SetEventOnCompletion(fence, this->m_fenceEvent), "Failed to set event completion");
            WaitForSingleObject(this->m_fenceEvent, INFINITE);
        }

        this->m_frameIndex = this->m_swapChain->GetCurrentBackBufferIndex();
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

    void D3D12Window::CreateRenderTarget() {
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(this->m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

        for (UINT n = 0; n < FrameCount; n++) {
            LogOnError(this->m_swapChain->GetBuffer(n, IID_PPV_ARGS(&this->m_renderTargets[n])), "Failed to create Render Target View");
            this->m_device->CreateRenderTargetView(this->m_renderTargets[n].Get(), nullptr, rtvHandle);
            rtvHandle.Offset(1, this->m_rtvDescriptorSize);
        }
    }

    void D3D12Window::CleanupRenderTarget() {
        for (UINT n = 0; n < FrameCount; n++) {
            if (this->m_renderTargets[n]) {
                this->m_renderTargets[n].Reset();
                this->m_renderTargets[n] = nullptr;
            }
        }
    }

    void D3D12Window::LogOnError(HRESULT hr, char err[]) {
        if (hr != S_OK) {
            PG_CORE_CRITICAL(err);
        }
    }

    void D3D12Window::BeforeUpdate() {
        RECT winRect;
        GetClientRect(this->m_Window, &winRect);

        D3D12_VIEWPORT viewport = {
            0.0f,
            0.0f,
            (FLOAT)(winRect.right - winRect.left),
            (FLOAT)(winRect.bottom - winRect.top),
            0.0f,
            1.0f};

        MSG msg;

        // Check to see if any messages are waiting in the queue
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            // translate keystroke messages into the right format
            TranslateMessage(&msg);

            // send the message to the WindowProc function
            // DispatchMessage(&msg);
            this->InternalWindowProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);

            // check to see if it's time to quit
            if (msg.message == WM_QUIT) {
                Base::WindowCloseEvent e = Base::WindowCloseEvent();
                this->m_windowData.EventCallback(e);
            }
        }

        LogOnError(this->m_commandAllocator->Reset(), "Failed to reset Command Allocator");
        LogOnError(this->m_commandList->Reset(this->m_commandAllocator.Get(), NULL), "Failed to reset Command List");

        // Indicate that the back buffer will be used as a render target.
        this->m_barrier = CD3DX12_RESOURCE_BARRIER::Transition(this->m_renderTargets[this->m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
        this->m_commandList->ResourceBarrier(1, &this->m_barrier);

        // Set necessary state.
        this->m_commandList->SetGraphicsRootSignature(this->m_rootSignature.Get());
        this->m_commandList->RSSetViewports(1, &viewport);
        this->m_commandList->RSSetScissorRects(1, &winRect);

        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(this->m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), this->m_frameIndex, this->m_rtvDescriptorSize);
        this->m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

        // Record commands.
        const float clearColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
        this->m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
        this->m_commandList->SetDescriptorHeaps(1, m_srvHeap.GetAddressOf());
        this->m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    void D3D12Window::OnUpdate() {
        // Indicate that the back buffer will now be used to present.
        this->m_barrier = CD3DX12_RESOURCE_BARRIER::Transition(this->m_renderTargets[this->m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
        this->m_commandList->ResourceBarrier(1, &this->m_barrier);

        LogOnError(this->m_commandList->Close(), "Failed to close Command List");

        // Execute the command list.
        ID3D12CommandList* ppCommandLists[] = {this->m_commandList.Get()};
        this->m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

        // Present the frame.
        LogOnError(this->m_swapChain->Present(1, 0), "Failed to present frame");

        WaitForPreviousFrame();
    }
}
