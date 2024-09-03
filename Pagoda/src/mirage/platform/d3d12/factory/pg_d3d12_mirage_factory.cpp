#include "pgpch.h"
#include "pg_d3d12_mirage_factory.h"

#include "mirage/platform/d3d12/buffer/pg_d3d12_vertex_buffer.h"
#include "mirage/platform/d3d12/buffer/pg_d3d12_index_buffer.h"
#include "mirage/platform/d3d12/shader/pg_d3d12_shader.h"
#include "mirage/platform/d3d12/renderer/pg_d3d12_renderer.h"
#include "mirage/platform/d3d12/pipeline/pg_d3d12_pipeline_state.h"
#include "mirage/platform/d3d12/buffer/pg_d3d12_constant_buffer.h"

#include "chisel/platform/d3d12/extension/pg_chisel_d3d12_mirage_extension.h"

namespace Pagoda::Mirage {
    VertexBuffer* D3D12MirageFactory::CreateVertexBuffer(float buffer[], int bufferCount, int vertexCount, VertexBufferLayout& layout) const {
        return new D3D12VertexBuffer(m_context, buffer, bufferCount, vertexCount, layout);
    }

    IndexBuffer* D3D12MirageFactory::CreateIndexBuffer(unsigned int buffer[], int bufferCount) const {
        return new D3D12IndexBuffer(m_context, buffer, bufferCount);
    }

    Shader* D3D12MirageFactory::CreateShader(std::string& filePath, VertexBufferLayout& vertexBufferLayout, ShaderType shaderType) const {
        return new D3D12Shader(filePath, vertexBufferLayout, shaderType);
    }

    Renderer* D3D12MirageFactory::CreateRenderer() const {
        return new D3D12Renderer(m_context);
    }

    PipelineState* D3D12MirageFactory::CreatePipelineState(Shader* vertexShader, Shader* fragmentShader, VertexBufferLayout& vertexBufferLayout) const {
        return new D3D12PipelineState(m_context, vertexShader, fragmentShader, vertexBufferLayout);
    }

    ConstantBuffer<float>* D3D12MirageFactory::CreateTransformConstantBuffer(int size) const {
        return new D3D12ConstantBuffer<float>(m_context, size, ConstantBufferType::CONSTANT_BUFFER_TYPE_TRANSFORM);
    }

    ConstantBuffer<float>* D3D12MirageFactory::CreateTransformConstantBuffer(float buffer[], int size) const {
        return new D3D12ConstantBuffer<float>(m_context, buffer, size, ConstantBufferType::CONSTANT_BUFFER_TYPE_TRANSFORM);
    }

    Extension* D3D12MirageFactory::CreateChiselExtension() const {
        return new Chisel::ChiselD3D12MirageExtension(m_context);
    }

}