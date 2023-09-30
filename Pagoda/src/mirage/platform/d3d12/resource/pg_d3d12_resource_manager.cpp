#include "pgpch.h"
#include "pg_d3d12_resource_manager.h"

namespace Pagoda::Mirage {
    D3D12ResourceManager::D3D12ResourceManager() {
        this->m_device = D3D12Context().GetDevice();
        this->m_commandQueue = D3D12Context().GetCommandQueue();

        this->m_fenceValue = 1;

        this->m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (this->m_fenceEvent == nullptr) {
            HRESULT_FROM_WIN32(GetLastError());
        }

        HRESULT fr = this->m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&this->m_fence));

        HRESULT cmr = this->m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&this->m_commandAllocator));
        HRESULT clr = this->m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, this->m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&this->m_commandList));

        long res = fr & cmr & clr;

        PG_CORE_ASSERT(res == S_OK, "Failed to initalise D3D12ResourceManager");

        this->m_commandList->Close();
    }

    D3D12ResourceManager::~D3D12ResourceManager() {
    }

    void D3D12ResourceManager::CopyAndTransition(ID3D12Resource* dest, ID3D12Resource* src) {
        this->m_commandAllocator->Reset();
        this->m_commandList->Reset(this->m_commandAllocator.Get(), NULL);
        this->m_commandList->CopyResource(dest, src);

        this->m_barrier = CD3DX12_RESOURCE_BARRIER::Transition(dest, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
        this->m_commandList->ResourceBarrier(1, &m_barrier);

        this->m_commandList->Close();
        ID3D12CommandList* ppCommandLists[] = {this->m_commandList.Get()};
        this->m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

        const UINT64 fence = this->m_fenceValue;
        this->m_fenceValue++;
        HRESULT sr = this->m_commandQueue->Signal(this->m_fence.Get(), fence);
        PG_CORE_ASSERT(sr == S_OK, "Failed to request signal");

        if (this->m_fence->GetCompletedValue() < fence) {
            HRESULT er = this->m_fence->SetEventOnCompletion(fence, this->m_fenceEvent);
            PG_CORE_ASSERT(er == S_OK, "Failed to set event handle");
            WaitForSingleObject(this->m_fenceEvent, INFINITE);
        }
    }
}