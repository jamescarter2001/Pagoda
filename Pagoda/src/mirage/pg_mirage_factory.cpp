#include "pgpch.h"
#include "pg_mirage_factory.h"

namespace Pagoda::Mirage {
    VertexBuffer* MirageFactory::CreateVertexBuffer(float buffer[], int bufferCount, int vertexCount, VertexBufferLayout& layout) {
        switch (Renderer::GetRendererAPI()) {
            #ifdef PG_PLATFORM_WINDOWS
            case RendererAPI::Direct3D11:
                return new D3D11VertexBuffer(s_d3d11Context, buffer, bufferCount, vertexCount, layout);
            case RendererAPI::Direct3D12:
                return new D3D12VertexBuffer(s_d3d12Context, buffer, bufferCount, vertexCount, layout); 
            #endif
        }

        PG_CORE_ASSERT(false, "Unsupported Renderer API");
        return nullptr;
    }

    IndexBuffer* MirageFactory::CreateIndexBuffer(unsigned int buffer[], int bufferCount) {
        switch (Renderer::GetRendererAPI()) {
            #ifdef PG_PLATFORM_WINDOWS
            case RendererAPI::Direct3D11:
                return new D3D11IndexBuffer(s_d3d11Context, buffer, bufferCount);
            case RendererAPI::Direct3D12:
                return new D3D12IndexBuffer(s_d3d12Context, buffer, bufferCount);
            #endif
        }

        PG_CORE_ASSERT(false, "Unsupported Renderer API");
        return nullptr;
    }

    Shader* MirageFactory::CreateShader(std::string& filePath, VertexBufferLayout& vertexBufferLayout, ShaderType shaderType) {
        switch (Renderer::GetRendererAPI()) {
            #ifdef PG_PLATFORM_WINDOWS
            case RendererAPI::Direct3D11:
                return new D3D11Shader(s_d3d11Context, filePath, vertexBufferLayout, shaderType);
            case RendererAPI::Direct3D12:
                return new D3D12Shader(filePath, vertexBufferLayout, shaderType);
            #endif
        }

        PG_CORE_ASSERT(false, "Unsupported Renderer API");
        return nullptr;
    }

    Renderer* MirageFactory::CreateRenderer() {
        switch (Renderer::GetRendererAPI()) {
            #ifdef PG_PLATFORM_WINDOWS
            case RendererAPI::Direct3D11:
                return new D3D11Renderer(s_d3d11Context);
            case RendererAPI::Direct3D12:
                return new D3D12Renderer(s_d3d12Context);
            #endif
        }

        PG_CORE_ASSERT(false, "Unsupported Renderer API");
        return nullptr;
    }

    PipelineState* MirageFactory::CreatePipelineState(Shader* vertexShader, Shader* fragmentShader, VertexBufferLayout& vertexBufferLayout) {
        switch (Renderer::GetRendererAPI()) {
            #ifdef PG_PLATFORM_WINDOWS
            case RendererAPI::Direct3D11:
                return new D3D11PipelineState(s_d3d11Context, vertexShader, fragmentShader, vertexBufferLayout);
            case RendererAPI::Direct3D12:
                return new D3D12PipelineState(s_d3d12Context, vertexShader, fragmentShader, vertexBufferLayout);
            #endif
        }

        PG_CORE_ASSERT(false, "Unsupported Renderer API");
        return nullptr;
    }

    ConstantBuffer<float>* MirageFactory::CreateTransformConstantBuffer(int size) {
        switch (Renderer::GetRendererAPI()) {
            #ifdef PG_PLATFORM_WINDOWS
            case RendererAPI::Direct3D11:
                return new D3D11ConstantBuffer<float>(s_d3d11Context, size, ConstantBufferType::CONSTANT_BUFFER_TYPE_TRANSFORM);
            case RendererAPI::Direct3D12:
                return new D3D12ConstantBuffer<float>(s_d3d12Context, size, ConstantBufferType::CONSTANT_BUFFER_TYPE_TRANSFORM);
            #endif
        }

        PG_CORE_ASSERT(false, "Unsupported Renderer API");
        return nullptr;
    }

    ConstantBuffer<float>* MirageFactory::CreateTransformConstantBuffer(float buffer[], int size) {
        #ifdef PG_PLATFORM_WINDOWS
        switch (Renderer::GetRendererAPI()) {
            case RendererAPI::Direct3D11:
                return new D3D11ConstantBuffer<float>(s_d3d11Context, buffer, size, ConstantBufferType::CONSTANT_BUFFER_TYPE_TRANSFORM);
            case RendererAPI::Direct3D12:
                return new D3D12ConstantBuffer<float>(s_d3d12Context, buffer, size, ConstantBufferType::CONSTANT_BUFFER_TYPE_TRANSFORM);
        #endif
        }

        PG_CORE_ASSERT(false, "Unsupported Renderer API");
        return nullptr;
    }

    #ifdef PG_PLATFORM_WINDOWS
    D3D11Context* MirageFactory::s_d3d11Context = nullptr;
    D3D12Context* MirageFactory::s_d3d12Context = nullptr;

    void MirageFactory::SetD3D11Context(D3D11Context* ctx) { s_d3d11Context = ctx; }
    void MirageFactory::SetD3D12Context(D3D12Context* ctx) { s_d3d12Context = ctx; }
    #endif
}