#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"
#include "base/util/pg_strings.h"

#include "mirage/core/window/pg_window.h"
#include "mirage/core/factory/pg_mirage_factory.h"

#include "mirage/platform/d3d12/context/pg_d3d12_context.h"

namespace Pagoda::Mirage {
    class PAGODA_API D3D12Window : public Window {
    public:
        D3D12Window(const WindowProps& props, std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> winProcCallback);
        virtual ~D3D12Window();

        virtual inline std::string GetApiName() override {
            return "Direct3D12";
        }

        virtual std::shared_ptr<MirageFactory> Init() override;
        virtual void BeforeUpdate() override;
        virtual void OnUpdate() override;

        void WaitForPreviousFrame();

        void CreateRenderTarget();
        void CleanupRenderTarget();

    private:
        HWND m_Window;

        std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> m_winProcCallback = [](HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
            PG_CORE_WARNING("No WinProc callback set!");
            return false;
        };

        static const UINT FrameCount = 2;

        ComPtr<IDXGISwapChain3> m_swapChain;
        ComPtr<ID3D12Device> m_device;
        ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
        ComPtr<ID3D12CommandAllocator> m_commandAllocator;
        ComPtr<ID3D12CommandQueue> m_commandQueue;
        ComPtr<ID3D12RootSignature> m_rootSignature;
        ComPtr<ID3D12DescriptorHeap> m_srvHeap;
        ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
        ComPtr<ID3D12PipelineState> m_pipelineState;
        ComPtr<ID3D12GraphicsCommandList> m_commandList;

        UINT m_rtvDescriptorSize;

        // Synchronization objects.
        UINT m_frameIndex;
        HANDLE m_fenceEvent;
        ComPtr<ID3D12Fence> m_fence;
        UINT64 m_fenceValue;

        CD3DX12_RESOURCE_BARRIER m_barrier;

        // this is the main message handler for the program
        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
        LRESULT CALLBACK InternalWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
       
        static void GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter);

        std::shared_ptr<MirageFactory> Direct3D12Init();
        void LogOnError(HRESULT hr, char err[] = "Failed to initialize Direct3D12 context!");
    };
}