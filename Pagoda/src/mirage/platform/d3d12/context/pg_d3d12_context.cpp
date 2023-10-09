#include "pgpch.h"
#include "pg_d3d12_context.h"

namespace Pagoda::Mirage {
    D3D12Context::D3D12Context(ComPtr<IDXGISwapChain3> swapChain,
        ComPtr<ID3D12Device> device,
        ComPtr<ID3D12Resource> renderTargets[],
        ComPtr<ID3D12CommandAllocator> commandAllocator,
        ComPtr<ID3D12CommandQueue> commandQueue,
        ComPtr<ID3D12RootSignature> rootSignature,
        ComPtr<ID3D12DescriptorHeap> rtvHeap,
        ComPtr<ID3D12GraphicsCommandList> commandList) {

        m_swapChain = swapChain;
        m_device = device;
        m_commandAllocator = commandAllocator;
        m_commandQueue = commandQueue;
        m_rootSignature = rootSignature;
        m_rtvHeap = rtvHeap;
        m_commandList = commandList;
    }
}