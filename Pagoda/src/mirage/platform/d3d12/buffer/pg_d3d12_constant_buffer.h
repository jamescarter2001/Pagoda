#pragma once
#include "pgpch.h"

#include "mirage/core/buffer/pg_constant_buffer.h"

#include "mirage/platform/d3d12/context/pg_d3d12_context.h"
#include "mirage/platform/d3d12/resource/pg_d3d12_resource_allocator.h"

namespace Pagoda::Mirage {
    template<typename T>
    class D3D12ConstantBuffer : public ConstantBuffer<T> {
    public:
        D3D12ConstantBuffer(std::shared_ptr<D3D12Context> context, T buffer[], int size, ConstantBufferType type) : D3D12ConstantBuffer(context, size, type) {
            this->Write(buffer);
        }

        D3D12ConstantBuffer(std::shared_ptr<D3D12Context> context, int size, ConstantBufferType type) : ConstantBuffer(size, type), m_ctx(context) {
            D3D12ResourceAllocator ra(context);

            int alignedSize = 0;
            int remainder = (size % s_alignmentFactor);

            if (remainder == 0) {
                alignedSize = size;
            } else {
                alignedSize = size + (256 - remainder);
            }

            PG_CORE_ASSERT(alignedSize % s_alignmentFactor == 0, "Constant Buffer size is not aligned!");

            m_constantBuffer = ra.AllocateUpload(size);

            switch (type) {
                case ConstantBufferType::CONSTANT_BUFFER_TYPE_MVP:
                    this->m_slot = 0;
                    break;
                case ConstantBufferType::CONSTANT_BUFFER_TYPE_TRANSFORM:
                    this->m_slot = 1;
                    break;
                default:
                    this->m_slot = -1;
                    break;
            }
        }

        virtual ~D3D12ConstantBuffer() {}

        virtual void Bind() const override {
            this->m_ctx->GetCommandList()->SetGraphicsRootConstantBufferView(this->m_slot, this->m_constantBuffer->GetGPUVirtualAddress());
        }
        virtual void Unbind() const override {

        }

        virtual void Write(T buffer[]) const override {
            Buffer::Write(buffer);
            CD3DX12_RANGE readRange(0, 0);
            UINT8* pVertexDataBegin;
            m_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
            memcpy(pVertexDataBegin, buffer, this->GetSize());
            m_constantBuffer->Unmap(0, nullptr);
        }

    private:
        static inline constexpr unsigned int s_alignmentFactor = 256;

        std::shared_ptr<D3D12Context> m_ctx;

        ComPtr<ID3D12Resource> m_constantBuffer;

        int m_slot;
    };
}