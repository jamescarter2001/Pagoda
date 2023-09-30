#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "mirage/platform/d3d12/context/pg_d3d12_context.h"

namespace Pagoda::Mirage {
    class D3D12ResourceManager {
    public:
        D3D12ResourceManager();
        virtual ~D3D12ResourceManager();

        void CopyAndTransition(ID3D12Resource* dest, ID3D12Resource* src);

    private:
        ComPtr<ID3D12Device> m_device;
        ComPtr<ID3D12CommandQueue> m_commandQueue;
        ComPtr<ID3D12CommandAllocator> m_commandAllocator;
        ComPtr<ID3D12GraphicsCommandList> m_commandList;

        CD3DX12_RESOURCE_BARRIER m_barrier;

        ComPtr<ID3D12Fence> m_fence;
        UINT64 m_fenceValue;
        HANDLE m_fenceEvent;
    };
}