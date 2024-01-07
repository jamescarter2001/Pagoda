#include "pgpch.h"
#include "pg_d3d12_resource_allocator.h"

namespace Pagoda::Mirage {
    D3D12ResourceAllocator::D3D12ResourceAllocator(D3D12Context* context) : m_context(context) {
        this->m_fenceValue = 1;

        this->m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (this->m_fenceEvent == nullptr) {
            HRESULT_FROM_WIN32(GetLastError());
        }

        HRESULT fr = this->m_context->GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&this->m_fence));

        HRESULT cmr = this->m_context->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&this->m_commandAllocator));
        HRESULT clr = this->m_context->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, this->m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&this->m_commandList));

        long res = fr & cmr & clr;

        PG_CORE_ASSERT(res == S_OK, "Failed to initalise D3D12ResourceAllocator");

        this->m_commandList->Close();
    }

    D3D12ResourceAllocator::~D3D12ResourceAllocator() {
    }

    void D3D12ResourceAllocator::AllocateUpload(ID3D12Resource** res, int size) {
        CD3DX12_HEAP_PROPERTIES uploadProps(D3D12_HEAP_TYPE_UPLOAD);
        auto desc = CD3DX12_RESOURCE_DESC::Buffer(size);

        HRESULT ur = this->m_context->GetDevice()->CreateCommittedResource(
            &uploadProps,                       // a default heap
            D3D12_HEAP_FLAG_NONE,               // no flags
            &desc,                              // resource description for a buffer
            D3D12_RESOURCE_STATE_GENERIC_READ,  // start in the copy destination state
            nullptr,                            // optimized clear value must be null for this type of resource
            IID_PPV_ARGS(res));
    }

    void D3D12ResourceAllocator::AllocateDefault(ID3D12Resource** res, void* buff, int size) {
        CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_DEFAULT);
        CD3DX12_HEAP_PROPERTIES uploadProps(D3D12_HEAP_TYPE_UPLOAD);
        auto desc = CD3DX12_RESOURCE_DESC::Buffer(size);

        ComPtr<ID3D12Resource> uploadBuffer;

        HRESULT ur = this->m_context->GetDevice()->CreateCommittedResource(
            &uploadProps,                       // a default heap
            D3D12_HEAP_FLAG_NONE,               // no flags
            &desc,                              // resource description for a buffer
            D3D12_RESOURCE_STATE_GENERIC_READ,  // start in the copy destination state
            nullptr,                            // optimized clear value must be null for this type of resource
            IID_PPV_ARGS(&uploadBuffer));

        HRESULT hr = this->m_context->GetDevice()->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(res));

        if (hr != S_OK || ur != S_OK) {
            PG_CORE_ERROR("Failed to allocate Committed Resources");
            return;
        }

        // Copy the triangle data to the vertex buffer.
        UINT8* pVertexDataBegin;
        CD3DX12_RANGE readRange(0, 0);  // No intention to read from this resource on the CPU.
        uploadBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
        memcpy(pVertexDataBegin, buff, size);
        uploadBuffer->Unmap(0, nullptr);

        this->CopyAndTransition(*res, uploadBuffer.Get());

        PG_CORE_DEBUG("Allocated GPU resource with size: {}", size);
    }

    void D3D12ResourceAllocator::CopyAndTransition(ID3D12Resource* dest, ID3D12Resource* src) {
        this->m_commandAllocator->Reset();
        this->m_commandList->Reset(this->m_commandAllocator.Get(), NULL);
        this->m_commandList->CopyResource(dest, src);

        this->m_barrier = CD3DX12_RESOURCE_BARRIER::Transition(dest, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
        this->m_commandList->ResourceBarrier(1, &m_barrier);

        this->m_commandList->Close();
        ID3D12CommandList* ppCommandLists[] = {this->m_commandList.Get()};
        this->m_context->GetCommandQueue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

        const UINT64 fence = this->m_fenceValue;
        this->m_fenceValue++;
        HRESULT sr = this->m_context->GetCommandQueue()->Signal(this->m_fence.Get(), fence);
        PG_CORE_ASSERT(sr == S_OK, "Failed to request signal");

        if (this->m_fence->GetCompletedValue() < fence) {
            HRESULT er = this->m_fence->SetEventOnCompletion(fence, this->m_fenceEvent);
            PG_CORE_ASSERT(er == S_OK, "Failed to set event handle");
            WaitForSingleObject(this->m_fenceEvent, INFINITE);
        }
    }
}