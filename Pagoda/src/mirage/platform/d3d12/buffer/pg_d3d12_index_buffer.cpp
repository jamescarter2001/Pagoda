#include "pgpch.h"
#include "pg_d3d12_index_buffer.h"

namespace Pagoda::Mirage {
    D3D12IndexBuffer::D3D12IndexBuffer(int buffer[], int size) : IndexBuffer(buffer, size) {

    }

    D3D12IndexBuffer::~D3D12IndexBuffer() {}

	void D3D12IndexBuffer::Bind() const {

    }

	void D3D12IndexBuffer::Unbind() const {

    }
}