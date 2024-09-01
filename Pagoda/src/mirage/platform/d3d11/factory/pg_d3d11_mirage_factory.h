#pragma once
#include "pgpch.h"
#include "mirage/core/factory/pg_mirage_factory.h"

#include "mirage/platform/d3d11/context/pg_d3d11_context.h"

namespace Pagoda::Mirage {
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
}