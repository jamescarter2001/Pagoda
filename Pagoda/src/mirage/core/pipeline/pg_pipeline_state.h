#pragma once
#include "pgpch.h"

#include "mirage/core/shader/pg_shader.h"

namespace Pagoda::Mirage {
    class PipelineState {
    public:
        PipelineState(Shader* vertexShader, Shader* fragmentShader, VertexBufferLayout& vertexBufferLayout) : m_vertexShader(vertexShader), m_fragmentShader(fragmentShader), m_vertexBufferLayout(vertexBufferLayout) {}
        virtual ~PipelineState() {}

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

    protected:
        VertexBufferLayout m_vertexBufferLayout;
        Shader* m_vertexShader;
        Shader* m_fragmentShader;
    };
}