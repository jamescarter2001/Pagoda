#pragma once
#include "pgpch.h"

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
    class PAGODA_API MirageFactory {
    public:
        MirageFactory(WindowData* wd)
            : m_windowData(wd) {
        }

        virtual ~MirageFactory() {
        }

        virtual VertexBuffer* CreateVertexBuffer(float buffer[], int bufferCount, int vertexCount, VertexBufferLayout& layout) const = 0;
        virtual IndexBuffer* CreateIndexBuffer(unsigned int buffer[], int bufferCount) const = 0;
        virtual Shader* CreateShader(std::string& filePath, VertexBufferLayout& vertexBufferLayout, ShaderType shaderType) const = 0;
        virtual Renderer* CreateRenderer() const = 0;
        virtual PipelineState* CreatePipelineState(Shader* vertexShader, Shader* fragmentShader, VertexBufferLayout& vertexBufferLayout) const = 0;

        virtual ConstantBuffer<float>* CreateTransformConstantBuffer(float buffer[], int size) const = 0;
        virtual ConstantBuffer<float>* CreateTransformConstantBuffer(int size) const = 0;

    protected:
        WindowData* m_windowData;
    };

#ifdef PG_PLATFORM_WINDOWS

    // Direct3D11
    class PAGODA_API D3D11MirageFactory : public MirageFactory {
    public:
        D3D11MirageFactory(WindowData* wd, std::shared_ptr<D3D11Context> c)
            : MirageFactory(wd), m_context(c) {
        }

        virtual VertexBuffer* CreateVertexBuffer(float buffer[], int bufferCount, int vertexCount, VertexBufferLayout& layout) const override;
        virtual IndexBuffer* CreateIndexBuffer(unsigned int buffer[], int bufferCount) const override;
        virtual Shader* CreateShader(std::string& filePath, VertexBufferLayout& vertexBufferLayout, ShaderType shaderType) const override;
        virtual Renderer* CreateRenderer() const override;
        virtual PipelineState* CreatePipelineState(Shader* vertexShader, Shader* fragmentShader, VertexBufferLayout& vertexBufferLayout) const override;

        ConstantBuffer<float>* CreateTransformConstantBuffer(int size) const override;
        ConstantBuffer<float>* CreateTransformConstantBuffer(float buffer[], int size) const override;

    private:
        std::shared_ptr<D3D11Context> m_context;
    };

    // Direct3D12
    class PAGODA_API D3D12MirageFactory : public MirageFactory {
    public:
        D3D12MirageFactory(WindowData* wd, std::shared_ptr<D3D12Context> c)
            : MirageFactory(wd), m_context(c) {
        }

        virtual ~D3D12MirageFactory() {
        }

        virtual VertexBuffer* CreateVertexBuffer(float buffer[], int bufferCount, int vertexCount, VertexBufferLayout& layout) const override;
        virtual IndexBuffer* CreateIndexBuffer(unsigned int buffer[], int bufferCount) const override;
        virtual Shader* CreateShader(std::string& filePath, VertexBufferLayout& vertexBufferLayout, ShaderType shaderType) const override;
        virtual Renderer* CreateRenderer() const override;
        virtual PipelineState* CreatePipelineState(Shader* vertexShader, Shader* fragmentShader, VertexBufferLayout& vertexBufferLayout) const override;

        ConstantBuffer<float>* CreateTransformConstantBuffer(int size) const override;
        ConstantBuffer<float>* CreateTransformConstantBuffer(float buffer[], int size) const override;

    private:
        std::shared_ptr<D3D12Context> m_context;
    };
#endif
}