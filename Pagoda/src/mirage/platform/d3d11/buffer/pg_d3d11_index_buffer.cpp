#include "pgpch.h"
#include "pg_d3d11_index_buffer.h"

namespace Pagoda::Mirage {
    D3D11IndexBuffer::D3D11IndexBuffer(const D3D11Context& context, unsigned int buffer[], const int size)
        : IndexBuffer(buffer, size) {
        this->m_device = context.GetDevicePtr();
        this->m_deviceContext = context.GetDeviceContextPtr();

        D3D11ResourceAllocator(context).AllocateDefault(&this->m_indexBufferPtr, buffer, size, D3D11_BIND_INDEX_BUFFER);
        PG_CORE_ASSERT(this->m_indexBufferPtr != NULL, "Index buffer pointer should not be null!");
    }

    D3D11IndexBuffer::~D3D11IndexBuffer() {
    }

    void D3D11IndexBuffer::Bind() const {
        this->m_deviceContext->IASetIndexBuffer(
            this->m_indexBufferPtr,
            DXGI_FORMAT_R32_UINT,
            0
            );
    }

    void D3D11IndexBuffer::Unbind() const {
    }
}
