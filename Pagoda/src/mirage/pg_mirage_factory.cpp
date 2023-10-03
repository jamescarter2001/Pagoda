#include "pgpch.h"
#include "pg_mirage_factory.h"

namespace Pagoda::Mirage {
    #ifdef PG_PLATFORM_WINDOWS
    D3D12Context MirageFactory::s_context;

    void MirageFactory::Init() {
        s_context = D3D12Context();
    }

    Window* MirageFactory::CreateContext(WindowProps& wp) {
        return new D3D12Window(wp);
    }

    VertexBuffer* MirageFactory::CreateVertexBuffer(float buffer[], int bufferCount, int vertexCount, VertexBufferLayout& layout) {
        return new D3D12VertexBuffer(s_context, buffer, bufferCount, vertexCount, layout);
    }

    IndexBuffer* MirageFactory::CreateIndexBuffer(int buffer[], int bufferCount) {
        return new D3D12IndexBuffer(s_context, buffer, bufferCount);
    }

    Shader* MirageFactory::CreateShader(std::string& filePath, VertexBufferLayout& vertexBufferLayout, ShaderType shaderType) {
        return new D3D12Shader(filePath, vertexBufferLayout, shaderType);
    }

    Renderer* MirageFactory::CreateRenderer() {
        return new D3D12Renderer(s_context);
    }

    PipelineState* MirageFactory::CreatePipelineState(Shader* vertexShader, Shader* fragmentShader, VertexBufferLayout& vertexBufferLayout) {
        return new D3D12PipelineState(s_context, vertexShader, fragmentShader, vertexBufferLayout);
    }
    #endif
}