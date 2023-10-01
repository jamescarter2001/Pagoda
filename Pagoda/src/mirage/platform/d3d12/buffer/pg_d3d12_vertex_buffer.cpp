#include "pgpch.h"
#include "pg_d3d12_vertex_buffer.h"

namespace Pagoda::Mirage {
    D3D12VertexBuffer::D3D12VertexBuffer(float buffer[], int size, int vertexCount, VertexBufferLayout vertexBufferLayout) : VertexBuffer(buffer, size, vertexCount, vertexBufferLayout) {
        D3D12Context context = D3D12Context();

        this->m_commandList = context.GetCommandList();

        std::unique_ptr<D3D12ResourceAllocator> ra = std::make_unique<D3D12ResourceAllocator>();
        ra->Allocate(&m_vertexBuffer, buffer, size);

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