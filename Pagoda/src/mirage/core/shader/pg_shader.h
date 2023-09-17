#pragma once
#include "pgpch.h"

#include "mirage/core/buffer/pg_vertex_buffer_layout.h"

namespace Pagoda::Mirage {
    enum class ShaderType {
        SHADER_TYPE_VERTEX,
        SHADER_TYPE_FRAGMENT
    };

    class Shader {
    public:
        Shader(std::string filePath, VertexBufferLayout vertexBufferLayout, ShaderType& shaderType) : m_FilePath(filePath), m_VertexBufferLayout(vertexBufferLayout), m_ShaderType(shaderType) {}
        virtual ~Shader() {}

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

    protected:
        std::string m_FilePath;
        VertexBufferLayout m_VertexBufferLayout;

        ShaderType m_ShaderType;
    };

    struct ShaderData {
        Shader* vertexShader;
        Shader* fragmentShader;
    };
}