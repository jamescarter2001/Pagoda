#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "mirage/platform/d3d12/context/pg_d3d12_context.h"

namespace Pagoda::Mirage {
    class D3D12ResourceAllocator {
    public:
        D3D12ResourceAllocator(std::shared_ptr<D3D12Context> context);
        virtual ~D3D12ResourceAllocator();

        ComPtr<ID3D12Resource> AllocateCommittedResource(const unsigned int size, const D3D12_HEAP_TYPE heapType, const D3D12_RESOURCE_STATES initialState);
        ComPtr<ID3D12Resource> AllocateDefault(void* buff, int size);
        ComPtr<ID3D12Resource> AllocateUpload(int size);

    private:
        void CopyAndTransition(ComPtr<ID3D12Resource> dest, ComPtr<ID3D12Resource> src);

        std::shared_ptr<D3D12Context> m_ctx;

        ComPtr<ID3D12CommandAllocator> m_commandAllocator;
        ComPtr<ID3D12GraphicsCommandList> m_commandList;

        CD3DX12_RESOURCE_BARRIER m_barrier;

        ComPtr<ID3D12Fence> m_fence;
        UINT64 m_fenceValue;
        HANDLE m_fenceEvent;
    };
}