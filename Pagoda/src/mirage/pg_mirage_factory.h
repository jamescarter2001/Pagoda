#pragma once
#include "pgpch.h"

#include "mirage/core/window/pg_window.h"

#include "mirage/core/buffer/pg_buffer.h"
#include "mirage/core/shader/pg_shader.h"
#include "mirage/core/renderer/pg_renderer.h"

// Windows

#ifdef PG_PLATFORM_WINDOWS

#include "mirage/platform/d3d11/window/pg_d3d11_window.h"
#include "mirage/platform/d3d11/buffer/pg_d3d11_vertex_buffer.h"
#include "mirage/platform/d3d11/buffer/pg_d3d11_index_buffer.h"
#include "mirage/platform/d3d11/shader/pg_d3d11_shader.h"
#include "mirage/platform/d3d11/renderer/pg_d3d11_renderer.h"
#include "mirage/platform/d3d11/pipeline/pg_d3d11_pipeline_state.h"

#include "mirage/platform/d3d12/window/pg_d3d12_window.h"
#include "mirage/platform/d3d12/buffer/pg_d3d12_vertex_buffer.h"
#include "mirage/platform/d3d12/buffer/pg_d3d12_index_buffer.h"
#include "mirage/platform/d3d12/shader/pg_d3d12_shader.h"
#include "mirage/platform/d3d12/renderer/pg_d3d12_renderer.h"
#include "mirage/platform/d3d12/pipeline/pg_d3d12_pipeline_state.h"

#endif

namespace Pagoda::Mirage {
    class PAGODA_API MirageFactory {
    public:
        static Window* CreateContext(WindowProps& wp);
        static VertexBuffer* CreateVertexBuffer(float buffer[], int bufferCount, int vertexCount, VertexBufferLayout& layout);
        static IndexBuffer* CreateIndexBuffer(int buffer[], int bufferCount);
        static Shader* CreateShader(std::string& filePath, VertexBufferLayout& vertexBufferLayout, ShaderType shaderType);
        static Renderer* CreateRenderer();
        static PipelineState* CreatePipelineState(Shader* vertexShader, Shader* fragmentShader, VertexBufferLayout& vertexBufferLayout);
    };
}