#pragma once
#include "pgpch.h"

namespace Pagoda::Mirage {
    class D3D12Context {
    public:
        D3D12Context() {}
        ~D3D12Context() {}

        static void Init(ComPtr<IDXGISwapChain3> swapChain,
                     ComPtr<ID3D12Device> device,
                     ComPtr<ID3D12Resource> renderTargets[],
                     ComPtr<ID3D12CommandAllocator> commandAllocator,
                     ComPtr<ID3D12CommandQueue> commandQueue,
                     ComPtr<ID3D12RootSignature> rootSignature,
                     ComPtr<ID3D12DescriptorHeap> rtvHeap,
                     ComPtr<ID3D12GraphicsCommandList> commandList);

        void SetActiveCommandList(ComPtr<ID3D12GraphicsCommandList> commandList) {
            this->s_commandList = commandList;
        }

        inline ComPtr<ID3D12Device> GetDevice() const& {
            return s_device;
        }

        inline ComPtr<ID3D12CommandQueue> GetCommandQueue() const& {
            return s_commandQueue;
        }

        inline ComPtr<ID3D12GraphicsCommandList> GetCommandList() const& {
            return s_commandList;
        }

        inline ComPtr<ID3D12RootSignature> GetRootSignature() const& {
            return s_rootSignature;
        }

    private:
        static ComPtr<IDXGISwapChain3> s_swapChain;
        static ComPtr<ID3D12Device> s_device;
        static ComPtr<ID3D12Resource> s_renderTargets[];
        static ComPtr<ID3D12CommandAllocator> s_commandAllocator;
        static ComPtr<ID3D12CommandQueue> s_commandQueue;
        static ComPtr<ID3D12RootSignature> s_rootSignature;
        static ComPtr<ID3D12DescriptorHeap> s_rtvHeap;
        static ComPtr<ID3D12PipelineState> s_pipelineState;
        static ComPtr<ID3D12GraphicsCommandList> s_commandList;
    };
}