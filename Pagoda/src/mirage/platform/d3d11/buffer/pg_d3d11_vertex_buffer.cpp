#include "pgpch.h"
#include "pg_d3d11_vertex_buffer.h"

namespace Pagoda::Mirage {
    D3D11VertexBuffer::D3D11VertexBuffer(ID3D11Device* device, float* buffer, int count) : VertexBuffer(buffer, count), m_Device(device) {
        this->m_Device = device;
        this->m_VertexBufferPtr = NULL;

        D3D11_BUFFER_DESC vertexBufferDescriptor = {};
        vertexBufferDescriptor.ByteWidth = count;
        vertexBufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA sr_data = {0};
        sr_data.pSysMem = buffer;
        HRESULT hr = m_Device->CreateBuffer(
            &vertexBufferDescriptor,
            &sr_data,
            &this->m_VertexBufferPtr);
        
        PG_CORE_ASSERT(hr == S_OK, "Failed to load vertex buffer");
    }

    D3D11VertexBuffer::~D3D11VertexBuffer() {
        this->m_VertexBufferPtr->Release();
    }

	void D3D11VertexBuffer::Bind() const {

    }

	void D3D11VertexBuffer::Unbind() const {

    }
}