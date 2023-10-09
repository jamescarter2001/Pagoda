#include "pgpch.h"
#include "pg_mirage_factory.h"

namespace Pagoda::Mirage {
    #ifdef PG_PLATFORM_WINDOWS

    // D3D11MirageFactory

    VertexBuffer* D3D11MirageFactory::CreateVertexBuffer(float buffer[], int bufferCount, int vertexCount, VertexBufferLayout& layout) const {
        return new D3D11VertexBuffer(this->m_context, buffer, bufferCount, vertexCount, layout);
    }

    IndexBuffer* D3D11MirageFactory::CreateIndexBuffer(unsigned int buffer[], int bufferCount) const {
        return new D3D11IndexBuffer(this->m_context, buffer, bufferCount);
    }

    Shader* D3D11MirageFactory::CreateShader(std::string& filePath, VertexBufferLayout& vertexBufferLayout, ShaderType shaderType) const {
        return new D3D11Shader(this->m_context, filePath, vertexBufferLayout, shaderType);
    }

    Renderer* D3D11MirageFactory::CreateRenderer() const {
        return new D3D11Renderer(this->m_context, this->m_windowData);
    }

    PipelineState* D3D11MirageFactory::CreatePipelineState(Shader* vertexShader, Shader* fragmentShader, VertexBufferLayout& vertexBufferLayout) const {
        return new D3D11PipelineState(this->m_context, vertexShader, fragmentShader, vertexBufferLayout);
    }

    ConstantBuffer<float>* D3D11MirageFactory::CreateTransformConstantBuffer(int size) const {
        return new D3D11ConstantBuffer<float>(m_context, size, ConstantBufferType::CONSTANT_BUFFER_TYPE_TRANSFORM);
    }

    ConstantBuffer<float>* D3D11MirageFactory::CreateTransformConstantBuffer(float buffer[], int size) const {
        return new D3D11ConstantBuffer<float>(m_context, buffer, size, ConstantBufferType::CONSTANT_BUFFER_TYPE_TRANSFORM);
    }

    // D3D12MirageFactory

    VertexBuffer* D3D12MirageFactory::CreateVertexBuffer(float buffer[], int bufferCount, int vertexCount, VertexBufferLayout& layout) const {
        return new D3D12VertexBuffer(this->m_context, buffer, bufferCount, vertexCount, layout);
    }

    IndexBuffer* D3D12MirageFactory::CreateIndexBuffer(unsigned int buffer[], int bufferCount) const {
        return new D3D12IndexBuffer(this->m_context, buffer, bufferCount);
    }

    Shader* D3D12MirageFactory::CreateShader(std::string& filePath, VertexBufferLayout& vertexBufferLayout, ShaderType shaderType) const {
        return new D3D12Shader(filePath, vertexBufferLayout, shaderType);
    }

    Renderer* D3D12MirageFactory::CreateRenderer() const {
        return new D3D12Renderer(this->m_context, this->m_windowData);
    }

    PipelineState* D3D12MirageFactory::CreatePipelineState(Shader* vertexShader, Shader* fragmentShader, VertexBufferLayout& vertexBufferLayout) const {
        return new D3D12PipelineState(this->m_context, vertexShader, fragmentShader, vertexBufferLayout);
    }

    ConstantBuffer<float>* D3D12MirageFactory::CreateTransformConstantBuffer(int size) const {
        return new D3D12ConstantBuffer<float>(m_context, size, ConstantBufferType::CONSTANT_BUFFER_TYPE_TRANSFORM);
    }

    ConstantBuffer<float>* D3D12MirageFactory::CreateTransformConstantBuffer(float buffer[], int size) const {
        return new D3D12ConstantBuffer<float>(m_context, buffer, size, ConstantBufferType::CONSTANT_BUFFER_TYPE_TRANSFORM);
    }
    #endif
}