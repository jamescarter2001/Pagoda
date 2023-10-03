#pragma once
#include "pgpch.h"

#include "mirage/core/buffer/pg_constant_buffer.h"

#include "mirage/platform/d3d12/context/pg_d3d12_context.h"
#include "mirage/platform/d3d12/resource/pg_d3d12_resource_allocator.h"

namespace Pagoda::Mirage {
    template<typename T>
    class D3D12ConstantBuffer : public ConstantBuffer<T> {
    public:
        D3D12ConstantBuffer(D3D12Context context, T buffer[], int size) : ConstantBuffer(buffer, size), m_context(context) {
            std::unique_ptr<D3D12ResourceAllocator> ra = std::make_unique<D3D12ResourceAllocator>();
            unsigned int alignMultiplier = (size / (1024 * 64)) + 1;

            unsigned int alignedSize = (1024 * 64) * alignMultiplier;
            ra->AllocateUpload(&m_constantBuffer, buffer, alignedSize);
        }

        virtual ~D3D12ConstantBuffer() {}

        virtual void Bind() const override {
            this->m_context.GetCommandList()->SetGraphicsRootConstantBufferView(0, this->m_constantBuffer->GetGPUVirtualAddress());
        }
        virtual void Unbind() const override {

        }

        virtual void Write(T buffer[]) const override {
            CD3DX12_RANGE readRange(0, 0);
            UINT8* pVertexDataBegin;
            m_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
            memcpy(pVertexDataBegin, buffer, this->GetSize());
            m_constantBuffer->Unmap(0, nullptr);
        }

    private:
        D3D12Context m_context;
        ID3D12Resource* m_constantBuffer;
    };
}