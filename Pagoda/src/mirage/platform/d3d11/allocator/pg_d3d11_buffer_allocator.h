#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

namespace Pagoda::Mirage {
    class PAGODA_API D3D11BufferAllocator {
    public:
        template <typename T>
        static void Allocate(ID3D11Buffer*& ptr, T buffer[], int bufferCount, D3D11_BIND_FLAG bufferType) {
            ptr = NULL;

            D3D11Context context = D3D11Context();

            D3D11_BUFFER_DESC vertexBufferDescriptor = {};
            vertexBufferDescriptor.ByteWidth = sizeof(T) * bufferCount;
            vertexBufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
            vertexBufferDescriptor.BindFlags = bufferType;
            D3D11_SUBRESOURCE_DATA sr_data = {0};
            sr_data.pSysMem = buffer;
            HRESULT hr = context.GetDevicePtr()->CreateBuffer(
                &vertexBufferDescriptor,
                &sr_data,
                &ptr);

            PG_CORE_ASSERT(hr == S_OK, "Failed to allocate D3D11 buffer");
        }
    };
}