#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "mirage/core/buffer/pg_index_buffer.h"

#include "mirage/platform/d3d11/context/pg_d3d11_context.h"
#include "mirage/platform/d3d11/allocator/pg_d3d11_buffer_allocator.h"

namespace Pagoda::Mirage {
    class PAGODA_API D3D11IndexBuffer : public IndexBuffer {
    public:
        D3D11IndexBuffer(D3D11Context context, unsigned int buffer[], int size);
        virtual ~D3D11IndexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;
    private:
        ID3D11Device* m_Device = NULL;
        ID3D11DeviceContext* m_DeviceContext = NULL;
        ID3D11Buffer* m_IndexBufferPtr = NULL;
    };
}