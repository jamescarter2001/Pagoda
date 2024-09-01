#include "pgpch.h"
#include "pg_d3d11_index_buffer.h"

namespace Pagoda::Mirage {
    D3D11IndexBuffer::D3D11IndexBuffer(std::shared_ptr<D3D11Context> ctx, unsigned int buffer[], const int size)
        : IndexBuffer(buffer, size) {
        this->m_device = ctx->GetDevice();
        this->m_deviceContext = ctx->GetDeviceContext();

        D3D11ResourceAllocator(ctx).AllocateDefault(&this->m_pIndexBuffer, buffer, size, D3D11_BIND_INDEX_BUFFER);
        PG_CORE_ASSERT(this->m_pIndexBuffer != NULL, "Index buffer pointer should not be null!");
    }

    D3D11IndexBuffer::~D3D11IndexBuffer() {
    }

    void D3D11IndexBuffer::Bind() const {
        this->m_deviceContext->IASetIndexBuffer(
            this->m_pIndexBuffer,
            DXGI_FORMAT_R32_UINT,
            0
            );
    }

    void D3D11IndexBuffer::Unbind() const {
    }
}
