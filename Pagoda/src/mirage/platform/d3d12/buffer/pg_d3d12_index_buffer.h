#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "mirage/core/buffer/pg_index_buffer.h"

#include "mirage/platform/d3d12/context/pg_d3d12_context.h"
#include "mirage/platform/d3d12/resource/pg_d3d12_resource_allocator.h"

namespace Pagoda::Mirage {
    class PAGODA_API D3D12IndexBuffer : public IndexBuffer {
    public:
        D3D12IndexBuffer(int buffer[], int size);
        virtual ~D3D12IndexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;
    private:
        ComPtr<ID3D12GraphicsCommandList> m_commandList;
        ComPtr<ID3D12Resource> m_indexBuffer;
        D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
    };
}