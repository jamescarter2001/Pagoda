#include "pgpch.h"
#include "pg_d3d11_resource_allocator.h"

namespace Pagoda::Mirage {
    D3D11ResourceAllocator::D3D11ResourceAllocator(D3D11Context context) : m_context(context) {
    }

    D3D11ResourceAllocator::~D3D11ResourceAllocator() {
    }

    void D3D11ResourceAllocator::AllocateDefault(ID3D11Buffer** ptr, void* buff, int size, D3D11_BIND_FLAG bufferType) {
        *ptr = NULL;

        D3D11_BUFFER_DESC desc = {};
        desc.ByteWidth = size;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = bufferType;
        D3D11_SUBRESOURCE_DATA sr_data = {0};
        sr_data.pSysMem = buff;
        HRESULT hr = this->m_context.GetDevicePtr()->CreateBuffer(
            &desc,
            &sr_data,
            ptr);

        PG_CORE_ASSERT(hr == S_OK, "Failed to allocate D3D11 Resource");
    }

    void D3D11ResourceAllocator::AllocateReadWrite(ID3D11Buffer** ptr, int size, D3D11_BIND_FLAG bufferType) {
        *ptr = NULL;

        D3D11_BUFFER_DESC desc = {};
        desc.ByteWidth = size;
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = bufferType;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        HRESULT hr = this->m_context.GetDevicePtr()->CreateBuffer(
            &desc,
            nullptr,
            ptr);

        PG_CORE_ASSERT(hr == S_OK, "Failed to allocate D3D11 Resource");
    }
}