#pragma once
#include "pgpch.h"

namespace Pagoda::Mirage {
    class D3D12Context {
    public:
        D3D12Context(ComPtr<IDXGISwapChain3> swapChain,
                     ComPtr<ID3D12Device> device,
                     ComPtr<ID3D12Resource> renderTargets[],
                     ComPtr<ID3D12CommandAllocator> commandAllocator,
                     ComPtr<ID3D12CommandQueue> commandQueue,
                     ComPtr<ID3D12RootSignature> rootSignature,
                     ComPtr<ID3D12DescriptorHeap> rtvHeap,
                     ComPtr<ID3D12GraphicsCommandList> commandList);

        ~D3D12Context() {
        }

        void SetActiveCommandList(const ComPtr<ID3D12GraphicsCommandList>& commandList) {
            this->m_commandList = commandList;
        }

        inline ComPtr<ID3D12Device> GetDevice() const & {
            return m_device;
        }

        inline ComPtr<ID3D12CommandQueue> GetCommandQueue() const & {
            return m_commandQueue;
        }

        inline ComPtr<ID3D12GraphicsCommandList> GetCommandList() const & {
            return m_commandList;
        }

        inline ComPtr<ID3D12RootSignature> GetRootSignature() const & {
            return m_rootSignature;
        }

    private:
        ComPtr<IDXGISwapChain3> m_swapChain;
        ComPtr<ID3D12Device> m_device;
        ComPtr<ID3D12Resource>* m_renderTargets;
        ComPtr<ID3D12CommandAllocator> m_commandAllocator;
        ComPtr<ID3D12CommandQueue> m_commandQueue;
        ComPtr<ID3D12RootSignature> m_rootSignature;
        ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
        ComPtr<ID3D12PipelineState> m_pipelineState;
        ComPtr<ID3D12GraphicsCommandList> m_commandList;
    };
}
