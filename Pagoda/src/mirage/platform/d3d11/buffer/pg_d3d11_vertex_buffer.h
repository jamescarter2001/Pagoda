#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "mirage/core/buffer/pg_vertex_buffer.h"

#include "mirage/platform/d3d11/context/pg_d3d11_context.h"

namespace Pagoda::Mirage {
    class PAGODA_API D3D11VertexBuffer : public VertexBuffer {
    public:
        D3D11VertexBuffer(float* buffer, int count, VertexBufferLayout& vertexBufferLayout);
        virtual ~D3D11VertexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;
    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_DeviceContext;
        ID3D11Buffer* m_VertexBufferPtr;
    };
}