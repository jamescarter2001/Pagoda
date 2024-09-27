#include "pgpch.h"
#include "pg_d3d12_index_buffer.h"

namespace Pagoda::Mirage {
    D3D12IndexBuffer::D3D12IndexBuffer(std::shared_ptr<D3D12Context> context, unsigned int buffer[], int size) : IndexBuffer(buffer, size), m_ctx(context) {
        D3D12ResourceAllocator ra(context);
        m_indexBuffer = ra.AllocateDefault(buffer, size);

        // Initialize the index buffer view.
        m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
        m_indexBufferView.SizeInBytes = size;
        m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
    }

    D3D12IndexBuffer::~D3D12IndexBuffer() {}

	void D3D12IndexBuffer::Bind() const {
        this->m_ctx->GetCommandList()->IASetIndexBuffer(&this->m_indexBufferView);
    }

	void D3D12IndexBuffer::Unbind() const {

    }
}