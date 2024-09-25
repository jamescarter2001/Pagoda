#include "pgpch.h"
#include "pg_d3d11_mirage_factory.h"

#include "mirage/platform/d3d11/buffer/pg_d3d11_vertex_buffer.h"
#include "mirage/platform/d3d11/buffer/pg_d3d11_index_buffer.h"
#include "mirage/platform/d3d11/shader/pg_d3d11_shader.h"
#include "mirage/platform/d3d11/renderer/pg_d3d11_renderer.h"
#include "mirage/platform/d3d11/pipeline/pg_d3d11_pipeline_state.h"
#include "mirage/platform/d3d11/buffer/pg_d3d11_constant_buffer.h"

#include "chisel/platform/d3d11/extension/pg_chisel_d3d11_mirage_extension.h"

namespace Pagoda::Mirage {
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
        return new D3D11Renderer(this->m_context);
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

    Pagoda::Mirage::Extension* D3D11MirageFactory::CreateChiselExtension() const {
        return new Chisel::ChiselD3D11MirageExtension(m_context);
    }
}