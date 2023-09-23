#include "pgpch.h"
#include "pg_mirage_factory.h"

namespace Pagoda::Mirage {
    #ifdef PG_PLATFORM_WINDOWS
    Window* MirageFactory::CreateContext(WindowProps& wp) {
        return new D3D12Window(wp);
    }

    VertexBuffer* MirageFactory::CreateVertexBuffer(float buffer[], int bufferCount, int vertexCount, VertexBufferLayout& layout) {
        return new D3D12VertexBuffer(buffer, bufferCount, vertexCount, layout);
    }

    IndexBuffer* MirageFactory::CreateIndexBuffer(int buffer[], int bufferCount) {
        return new D3D12IndexBuffer(buffer, bufferCount);
    }

    Shader* MirageFactory::CreateShader(std::string& filePath, VertexBufferLayout& vertexBufferLayout, ShaderType shaderType) {
        return new D3D12Shader(filePath, vertexBufferLayout, shaderType);
    }

    Renderer* MirageFactory::CreateRenderer() {
        return new D3D12Renderer();
    }

    PipelineState* MirageFactory::CreatePipelineState(Shader* vertexShader, Shader* fragmentShader, VertexBufferLayout& vertexBufferLayout) {
        return new D3D12PipelineState(vertexShader, fragmentShader, vertexBufferLayout);
    }
    #endif
}