#pragma once
#include "pgpch.h"

#include "mirage/core/buffer/pg_constant_buffer.h"

#include "mirage/platform/d3d11/context/pg_d3d11_context.h"
#include "mirage/platform/d3d11/resource/pg_d3d11_resource_allocator.h"

namespace Pagoda::Mirage {
    template <typename T>
    class D3D11ConstantBuffer : public ConstantBuffer<T> {
    public:
        D3D11ConstantBuffer(std::shared_ptr<D3D11Context> ctx, T buffer[], int size, ConstantBufferType type) : D3D11ConstantBuffer(ctx, size, type) {
            this->Write(buffer);
        }

        D3D11ConstantBuffer(std::shared_ptr<D3D11Context> ctx, int size, ConstantBufferType type) : ConstantBuffer(size, type), m_context(ctx) {
            D3D11ResourceAllocator(m_context).AllocateReadWrite(&this->m_constantBuffer, size, D3D11_BIND_CONSTANT_BUFFER);
            PG_CORE_ASSERT(this->m_constantBuffer != NULL, "Constant buffer pointer should not be null!");

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

        virtual ~D3D11ConstantBuffer() {}

        virtual void Bind() const override {
            m_context->GetDeviceContext()->VSSetConstantBuffers(this->m_slot, 1, &this->m_constantBuffer);
        }

        virtual void Unbind() const override {
        }

        virtual void Write(T buffer[]) const override {
            Buffer::Write(buffer);

            D3D11_MAPPED_SUBRESOURCE resource;
            m_context->GetDeviceContext()->Map(this->m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
            memcpy(resource.pData, buffer, this->m_BufferSize);
            m_context->GetDeviceContext()->Unmap(this->m_constantBuffer, 0);
        }

    private:
        std::shared_ptr<D3D11Context> m_context;

        ID3D11Buffer* m_constantBuffer;

        int m_slot;
    };
}