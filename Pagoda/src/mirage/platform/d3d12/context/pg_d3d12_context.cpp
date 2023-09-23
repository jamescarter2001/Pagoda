#include "pgpch.h"
#include "pg_d3d12_context.h"

namespace Pagoda::Mirage {
    ComPtr<IDXGISwapChain3> D3D12Context::s_swapChain;
    ComPtr<ID3D12Device> D3D12Context::s_device;
    ComPtr<ID3D12CommandAllocator> D3D12Context::s_commandAllocator;
    ComPtr<ID3D12CommandQueue> D3D12Context::s_commandQueue;
    ComPtr<ID3D12RootSignature> D3D12Context::s_rootSignature;
    ComPtr<ID3D12DescriptorHeap> D3D12Context::s_rtvHeap;
    ComPtr<ID3D12PipelineState> D3D12Context::s_pipelineState;
    ComPtr<ID3D12GraphicsCommandList> D3D12Context::s_commandList;

    void D3D12Context::Init(ComPtr<IDXGISwapChain3> swapChain,
        ComPtr<ID3D12Device> device,
        ComPtr<ID3D12Resource> renderTargets[],
        ComPtr<ID3D12CommandAllocator> commandAllocator,
        ComPtr<ID3D12CommandQueue> commandQueue,
        ComPtr<ID3D12RootSignature> rootSignature,
        ComPtr<ID3D12DescriptorHeap> rtvHeap,
        ComPtr<ID3D12GraphicsCommandList> commandList) {

        s_swapChain = swapChain;
        s_device = device;
        s_commandAllocator = commandAllocator;
        s_commandQueue = commandQueue;
        s_rootSignature = rootSignature;
        s_rtvHeap = rtvHeap;
        s_commandList = commandList;
    }
}