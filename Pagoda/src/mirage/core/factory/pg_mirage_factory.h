#pragma once
#include "pgpch.h"

#include "mirage/core/window/pg_window_data.h"

#include "mirage/core/buffer/pg_buffer.h"
#include "mirage/core/shader/pg_shader.h"
#include "mirage/core/renderer/pg_renderer.h"
#include "mirage/core/extension/pg_extension.h"

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

        // Extensions.

        virtual Extension* CreateChiselExtension() const = 0;

    protected:
        WindowData* m_windowData;
    };
}