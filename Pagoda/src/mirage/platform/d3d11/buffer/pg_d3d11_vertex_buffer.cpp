#include "pgpch.h"
#include "pg_d3d11_vertex_buffer.h"

namespace Pagoda::Mirage {
    D3D11VertexBuffer::D3D11VertexBuffer(float buffer[], int bufferCount, int vertexCount, VertexBufferLayout vertexBufferLayout) : VertexBuffer(buffer, bufferCount, vertexCount, vertexBufferLayout) {
        D3D11Context context = D3D11Context();

        this->m_Device = context.GetDevicePtr();
        this->m_DeviceContext = context.GetDeviceContextPtr();

        D3D11BufferAllocator::Allocate<float>(this->m_VertexBufferPtr, buffer, bufferCount, D3D11_BIND_VERTEX_BUFFER);
        PG_CORE_ASSERT(this->m_VertexBufferPtr != NULL, "Vertex buffer pointer should not be null!");
    }

    D3D11VertexBuffer::~D3D11VertexBuffer() {

    }

	void D3D11VertexBuffer::Bind() const {
        this->m_DeviceContext->IASetPrimitiveTopology(
            D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        this->m_DeviceContext->IASetVertexBuffers(
            0,
            1,
            &this->m_VertexBufferPtr,
            this->m_VertexBufferLayout.GetStridePtr(),
            &this->ms_Offset);
    }

	void D3D11VertexBuffer::Unbind() const {

    }
}