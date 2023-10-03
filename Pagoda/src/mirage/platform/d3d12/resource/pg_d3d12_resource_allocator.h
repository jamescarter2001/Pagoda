#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "mirage/platform/d3d12/context/pg_d3d12_context.h"

namespace Pagoda::Mirage {
    class D3D12ResourceAllocator {
    public:
        D3D12ResourceAllocator();
        virtual ~D3D12ResourceAllocator();

        void AllocateDefault(ID3D12Resource** res, void* buff, int size);
        void AllocateUpload(ID3D12Resource** res, void* buff, int size);

    private:
        void CopyAndTransition(ID3D12Resource* dest, ID3D12Resource* src);

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