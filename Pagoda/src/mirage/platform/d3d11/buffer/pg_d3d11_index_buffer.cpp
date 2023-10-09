#include "pgpch.h"
#include "pg_d3d11_index_buffer.h"

namespace Pagoda::Mirage {
    D3D11IndexBuffer::D3D11IndexBuffer(D3D11Context context, unsigned int buffer[], int size) : IndexBuffer(buffer, size) {
        this->m_Device = context.GetDevicePtr();
        this->m_DeviceContext = context.GetDeviceContextPtr();

        D3D11ResourceAllocator(context).AllocateDefault(&this->m_IndexBufferPtr, buffer, size, D3D11_BIND_INDEX_BUFFER);
        PG_CORE_ASSERT(this->m_IndexBufferPtr != NULL, "Index buffer pointer should not be null!");
    }

    D3D11IndexBuffer::~D3D11IndexBuffer() {}

	void D3D11IndexBuffer::Bind() const {
        this->m_DeviceContext->IASetIndexBuffer(
            this->m_IndexBufferPtr,
            DXGI_FORMAT_R32_UINT,
            0
        );
    }

	void D3D11IndexBuffer::Unbind() const {

    }
}