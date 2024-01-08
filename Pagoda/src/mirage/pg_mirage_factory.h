#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "mirage/core/window/pg_window_data.h"

#include "mirage/core/buffer/pg_buffer.h"
#include "mirage/core/shader/pg_shader.h"
#include "mirage/core/renderer/pg_renderer.h"

// Windows

#ifdef PG_PLATFORM_WINDOWS

#include "mirage/platform/d3d11/buffer/pg_d3d11_vertex_buffer.h"
#include "mirage/platform/d3d11/buffer/pg_d3d11_index_buffer.h"
#include "mirage/platform/d3d11/buffer/pg_d3d11_constant_buffer.h"
#include "mirage/platform/d3d11/shader/pg_d3d11_shader.h"
#include "mirage/platform/d3d11/renderer/pg_d3d11_renderer.h"
#include "mirage/platform/d3d11/pipeline/pg_d3d11_pipeline_state.h"

#include "mirage/platform/d3d12/context/pg_d3d12_context.h"
#include "mirage/platform/d3d12/buffer/pg_d3d12_vertex_buffer.h"
#include "mirage/platform/d3d12/buffer/pg_d3d12_index_buffer.h"
#include "mirage/platform/d3d12/buffer/pg_d3d12_constant_buffer.h"
#include "mirage/platform/d3d12/shader/pg_d3d12_shader.h"
#include "mirage/platform/d3d12/renderer/pg_d3d12_renderer.h"
#include "mirage/platform/d3d12/pipeline/pg_d3d12_pipeline_state.h"

#endif

namespace Pagoda::Mirage {
    class MirageFactory {
    public:
        static VertexBuffer* CreateVertexBuffer(float buffer[], int bufferCount, int vertexCount, VertexBufferLayout& layout);
        static IndexBuffer* CreateIndexBuffer(unsigned int buffer[], int bufferCount);
        static Shader* CreateShader(std::string& filePath, VertexBufferLayout& vertexBufferLayout, ShaderType shaderType);
        static PipelineState* CreatePipelineState(Shader* vertexShader, Shader* fragmentShader, VertexBufferLayout& vertexBufferLayout);
        static Renderer* CreateRenderer();
        static ConstantBuffer<float>* CreateTransformConstantBuffer(float buffer[], int size);
        static ConstantBuffer<float>* CreateTransformConstantBuffer(int size);

        #ifdef PG_PLATFORM_WINDOWS
        static void SetD3D11Context(D3D11Context* ctx);
        static void SetD3D12Context(D3D12Context* ctx);
        #endif
    private:
        // TODO: Is there a better way of accessing these from the factory methods?
        #ifdef PG_PLATFORM_WINDOWS
        static D3D11Context* s_d3d11Context;
        static D3D12Context* s_d3d12Context;
        #endif
    };
}
