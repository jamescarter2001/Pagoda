#include "pgpch.h"
#include "pg_d3d12_index_buffer.h"

namespace Pagoda::Mirage {
    D3D12IndexBuffer::D3D12IndexBuffer(int buffer[], int size) : IndexBuffer(buffer, size) {
        D3D12Context context = D3D12Context();

        this->m_device = context.GetDevice();
        this->m_commandList = context.GetCommandList();

        // Note: using upload heaps to transfer static data like vert buffers is not
        // recommended. Every time the GPU needs it, the upload heap will be marshalled
        // over. Please read up on Default Heap usage. An upload heap is used here for
        // code simplicity and because there are very few verts to actually transfer.
        CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
        auto desc = CD3DX12_RESOURCE_DESC::Buffer(size);
        HRESULT hr = this->m_device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_indexBuffer));

        if (hr != S_OK) {
            PG_CORE_ERROR("Failed to allocate Committed Resource");
            return;
        }

        // Copy the triangle data to the index buffer.
        UINT8* pIndexDataBegin;
        CD3DX12_RANGE readRange(0, 0);  // We do not intend to read from this resource on the CPU.
        m_indexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pIndexDataBegin));
        memcpy(pIndexDataBegin, buffer, size);
        m_indexBuffer->Unmap(0, nullptr);

        // Initialize the index buffer view.
        m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
        m_indexBufferView.SizeInBytes = size;
        m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
    }

    D3D12IndexBuffer::~D3D12IndexBuffer() {}

	void D3D12IndexBuffer::Bind() const {
        this->m_commandList->IASetIndexBuffer(&this->m_indexBufferView);
    }

	void D3D12IndexBuffer::Unbind() const {

    }
}