#include "pgpch.h"
#include "pg_d3d11_vertex_buffer.h"

namespace Pagoda::Mirage {
    D3D11VertexBuffer::D3D11VertexBuffer(float* buffer, int count, VertexBufferLayout& vertexBufferLayout) : VertexBuffer(buffer, count, vertexBufferLayout) {
        D3D11Context context = D3D11Context();

        this->m_Device = context.GetDevicePtr();
        this->m_DeviceContext = context.GetDeviceContextPtr();

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
        unsigned int stride = this->m_VertexBufferLayout.GetStride();
        unsigned int offset = 0;

        this->m_DeviceContext->IASetPrimitiveTopology(
            D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        this->m_DeviceContext->IASetVertexBuffers(
            0,
            1,
            &this->m_VertexBufferPtr,
            &stride,
            &offset);
    }

	void D3D11VertexBuffer::Unbind() const {

    }
}