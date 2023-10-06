#include "pgpch.h"
#include "pg_mirage_factory.h"

namespace Pagoda::Mirage {
    #ifdef PG_PLATFORM_WINDOWS
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
    #endif
}