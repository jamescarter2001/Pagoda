#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "mirage/core/pipeline/pg_pipeline_state.h"

#include "mirage/platform/d3d11/shader/pg_d3d11_shader.h"

namespace Pagoda::Mirage {
    class D3D11PipelineState : public PipelineState {
    public:
        D3D11PipelineState(D3D11Context context, Shader* vertexShader, Shader* fragmentShader, VertexBufferLayout& vertexBufferLayout);
        virtual ~D3D11PipelineState();

        virtual void Bind() const override;
        virtual void Unbind() const override;

    private:
        ID3D11Device* m_device;
        ID3D11DeviceContext* m_deviceContext;

        ID3D11InputLayout* m_inputLayout;

        ID3D11VertexShader* m_vs = NULL;
        ID3D11PixelShader* m_ps = NULL;
    };
}