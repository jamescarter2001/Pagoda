#include "pgpch.h"
#include "pg_d3d11_vertex_buffer.h"

namespace Pagoda::Mirage {
    D3D11VertexBuffer::D3D11VertexBuffer(std::shared_ptr<D3D11Context> ctx, float buffer[], int size, int vertexCount, VertexBufferLayout vertexBufferLayout) : VertexBuffer(buffer, size, vertexCount, vertexBufferLayout) {
        this->m_Device = ctx->GetDevice();
        this->m_DeviceContext = ctx->GetDeviceContext();

        D3D11ResourceAllocator(ctx).AllocateDefault(&this->m_VertexBufferPtr, buffer, size, D3D11_BIND_VERTEX_BUFFER);
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
            this->m_vertexBufferLayout.GetStridePtr(),
            &this->ms_Offset);
    }

	void D3D11VertexBuffer::Unbind() const {

    }
}