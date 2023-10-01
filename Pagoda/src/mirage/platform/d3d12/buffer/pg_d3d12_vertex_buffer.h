#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "mirage/core/buffer/pg_vertex_buffer_layout.h"
#include "mirage/core/buffer/pg_vertex_buffer.h"

#include "mirage/platform/d3d/util/pg_d3d_util.h"

#include "mirage/platform/d3d12/context/pg_d3d12_context.h"
#include "mirage/platform/d3d12/resource/pg_d3d12_resource_allocator.h"

namespace Pagoda::Mirage {
    class PAGODA_API D3D12VertexBuffer : public VertexBuffer {
    public:
        D3D12VertexBuffer(float buffer[], int size, int vertexCount, VertexBufferLayout vertexBufferLayout);
        virtual ~D3D12VertexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;
    private:
        ComPtr<ID3D12GraphicsCommandList> m_commandList;
        ComPtr<ID3D12Resource> m_vertexBuffer;
        D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
    };
}