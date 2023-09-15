#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "mirage/core/buffer/pg_vertex_buffer.h"

namespace Pagoda::Mirage {
    class PAGODA_API D3D11VertexBuffer : public VertexBuffer {
    public:
        D3D11VertexBuffer(ID3D11Device* device, float* buffer, int count);
        virtual ~D3D11VertexBuffer();
    protected:
        void Bind() const override;
        void Unbind() const override;

    private:
        ID3D11Device* m_Device;
        ID3D11Buffer* m_VertexBufferPtr;
    };
}