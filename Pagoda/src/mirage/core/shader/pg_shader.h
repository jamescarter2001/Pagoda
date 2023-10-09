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
        Shader(std::string filePath, VertexBufferLayout& vertexBufferLayout, ShaderType shaderType) : m_FilePath(filePath), m_vertexBufferLayout(vertexBufferLayout), m_ShaderType(shaderType) {}
        virtual ~Shader() {}

        inline std::string GetShaderType() {
            return this->m_ShaderType == ShaderType::SHADER_TYPE_VERTEX ? "VERTEX" : "FRAGMENT";
        }

        inline bool IsVertexShader() {
            return this->m_ShaderType == ShaderType::SHADER_TYPE_VERTEX;
        }

        inline VertexBufferLayout GetLayout() const& {
            return m_vertexBufferLayout;
        }

    protected:
        std::string m_FilePath;
        VertexBufferLayout m_vertexBufferLayout;

        ShaderType m_ShaderType;
    };

    struct ShaderData {
        Shader* vertexShader;
        Shader* fragmentShader;
    };
}