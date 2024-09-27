#include "pgpch.h"
#include "pg_d3d12_resource_allocator.h"

namespace Pagoda::Mirage {
    D3D12ResourceAllocator::D3D12ResourceAllocator(std::shared_ptr<D3D12Context> context) : m_ctx(context) {
        this->m_fenceValue = 1;

        this->m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (this->m_fenceEvent == nullptr) {
            HRESULT_FROM_WIN32(GetLastError());
        }

        HRESULT fr = this->m_ctx->GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&this->m_fence));

        HRESULT cmr = this->m_ctx->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&this->m_commandAllocator));
        HRESULT clr = this->m_ctx->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, this->m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&this->m_commandList));

        long res = fr & cmr & clr;

        PG_CORE_ASSERT(res == S_OK, "Failed to initalize D3D12ResourceAllocator.");

        this->m_commandList->Close();
    }

    D3D12ResourceAllocator::~D3D12ResourceAllocator() {
    }

    ComPtr<ID3D12Resource> D3D12ResourceAllocator::AllocateCommittedResource(const unsigned int size, const D3D12_HEAP_TYPE heapType, const D3D12_RESOURCE_STATES initialState) {
        CD3DX12_HEAP_PROPERTIES heapProps(heapType);
        auto desc = CD3DX12_RESOURCE_DESC::Buffer(size);

        ComPtr<ID3D12Resource> pRes;

        HRESULT hr = this->m_ctx->GetDevice()->CreateCommittedResource(
            &heapProps,                         // a default heap
            D3D12_HEAP_FLAG_NONE,               // no flags
            &desc,                              // resource description for a buffer
            initialState,                              // start in the copy destination initialState
            nullptr,                            // optimized clear value must be null for this type of resource
            IID_PPV_ARGS(&pRes)
        );

        PG_CORE_ASSERT(hr == S_OK, "Failed to allocate Committed Resource.");
        PG_CORE_DEBUG("Allocated Committed Resource with size: {}", size);

        return pRes;
    }

    ComPtr<ID3D12Resource> D3D12ResourceAllocator::AllocateUpload(int size) {
        return AllocateCommittedResource(size, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ);
    }

    ComPtr<ID3D12Resource> D3D12ResourceAllocator::AllocateDefault(void* buff, int size) {

        ComPtr<ID3D12Resource> pUploadBuffer = AllocateUpload(size);
        ComPtr<ID3D12Resource> pResult = AllocateCommittedResource(size, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_COMMON);

        // Copy the triangle data to the vertex buffer.
        UINT8* pVertexDataBegin;
        CD3DX12_RANGE readRange(0, 0);  // No intention to read from this resource on the CPU.
        pUploadBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
        memcpy(pVertexDataBegin, buff, size);
        pUploadBuffer->Unmap(0, nullptr);

        this->CopyAndTransition(pResult, pUploadBuffer);

        return pResult;
    }

    void D3D12ResourceAllocator::CopyAndTransition(ComPtr<ID3D12Resource> dest, ComPtr<ID3D12Resource> src) {
        this->m_commandAllocator->Reset();
        this->m_commandList->Reset(this->m_commandAllocator.Get(), NULL);
        this->m_commandList->CopyResource(dest.Get(), src.Get());

        this->m_barrier = CD3DX12_RESOURCE_BARRIER::Transition(dest.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
        this->m_commandList->ResourceBarrier(1, &m_barrier);

        this->m_commandList->Close();
        ID3D12CommandList* ppCommandLists[] = {this->m_commandList.Get()};
        this->m_ctx->GetCommandQueue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

        const UINT64 fence = this->m_fenceValue;
        this->m_fenceValue++;
        HRESULT sr = this->m_ctx->GetCommandQueue()->Signal(this->m_fence.Get(), fence);
        PG_CORE_ASSERT(sr == S_OK, "Failed to request signal.");

        if (this->m_fence->GetCompletedValue() < fence) {
            HRESULT er = this->m_fence->SetEventOnCompletion(fence, this->m_fenceEvent);
            PG_CORE_ASSERT(er == S_OK, "Failed to set event handle.");
            WaitForSingleObject(this->m_fenceEvent, INFINITE);
        }
    }

}