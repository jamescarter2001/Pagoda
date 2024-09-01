#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "mirage/core/buffer/pg_index_buffer.h"

#include "mirage/platform/d3d11/context/pg_d3d11_context.h"
#include "mirage/platform/d3d11/resource/pg_d3d11_resource_allocator.h"

namespace Pagoda::Mirage {
    class PAGODA_API D3D11IndexBuffer : public IndexBuffer {
    public:
        D3D11IndexBuffer(std::shared_ptr<D3D11Context> ctx, unsigned int buffer[], int size);
        virtual ~D3D11IndexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;

    private:
        ID3D11Device* m_device = nullptr;
        ID3D11DeviceContext* m_deviceContext = nullptr;
        ID3D11Buffer* m_pIndexBuffer = nullptr;
    };
}
