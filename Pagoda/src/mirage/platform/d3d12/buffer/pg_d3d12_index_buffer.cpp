#include "pgpch.h"
#include "pg_d3d12_index_buffer.h"

namespace Pagoda::Mirage {
    D3D12IndexBuffer::D3D12IndexBuffer(int buffer[], int size) : IndexBuffer(buffer, size) {
        D3D12Context context = D3D12Context();

        this->m_commandList = context.GetCommandList();

        std::unique_ptr<D3D12ResourceAllocator> ra = std::make_unique<D3D12ResourceAllocator>();
        ra->Allocate(&m_indexBuffer, buffer, size);

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