#include "pgpch.h"
#include "pg_d3d12_vertex_buffer.h"

namespace Pagoda::Mirage {
    D3D12VertexBuffer::D3D12VertexBuffer(float buffer[], int size, int vertexCount, VertexBufferLayout vertexBufferLayout) : VertexBuffer(buffer, size, vertexCount, vertexBufferLayout) {
        D3D12Context context = D3D12Context();

        this->m_device = context.GetDevice();
        this->m_commandList = context.GetCommandList();

        // Note: using upload heaps to transfer static data like vert buffers is not
        // recommended. Every time the GPU needs it, the upload heap will be marshalled
        // over. Please read up on Default Heap usage. An upload heap is used here for
        // code simplicity and because there are very few verts to actually transfer.
        CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_DEFAULT);
        CD3DX12_HEAP_PROPERTIES uploadProps(D3D12_HEAP_TYPE_UPLOAD);
        auto desc = CD3DX12_RESOURCE_DESC::Buffer(size);

        ComPtr<ID3D12Resource> uploadBuffer;

        HRESULT ur = this->m_device->CreateCommittedResource(
            &uploadProps,                                       // a default heap
            D3D12_HEAP_FLAG_NONE,                               // no flags
            &desc,                                              // resource description for a buffer
            D3D12_RESOURCE_STATE_GENERIC_READ,                  // start in the copy destination state
            nullptr,                                            // optimized clear value must be null for this type of resource
            IID_PPV_ARGS(&uploadBuffer));

        HRESULT hr = this->m_device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&m_vertexBuffer));

        if (hr != S_OK || ur != S_OK) {
            PG_CORE_ERROR("Failed to allocate Committed Resources");
            return;
        }

        // Copy the triangle data to the vertex buffer.
        UINT8* pVertexDataBegin;
        CD3DX12_RANGE readRange(0, 0);  // We do not intend to read from this resource on the CPU.
        uploadBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
        memcpy(pVertexDataBegin, buffer, size);
        uploadBuffer->Unmap(0, nullptr);

        std::unique_ptr<D3D12ResourceManager> rm = std::make_unique<D3D12ResourceManager>();
        rm->CopyAndTransition(m_vertexBuffer.Get(), uploadBuffer.Get());

        // Initialize the vertex buffer view.
        m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
        m_vertexBufferView.StrideInBytes = vertexBufferLayout.GetStride();
        m_vertexBufferView.SizeInBytes = size;
    }

    D3D12VertexBuffer::~D3D12VertexBuffer() {

    }

	void D3D12VertexBuffer::Bind() const {
        this->m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
    }

	void D3D12VertexBuffer::Unbind() const {

    }
}