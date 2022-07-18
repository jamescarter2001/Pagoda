#pragma once

#include "pgpch.h"

#include "Math/pgMath.h"

namespace Pagoda::Mirage {
    struct ShaderProgramSource {
        std::string vertexShader;
        std::string fragmentShader;
    };

    enum class ShaderType {
        NONE = -1,
        VERTEX = 1,
        FRAGMENT = 2
    };

    class Shader {
    protected:
        unsigned int m_RendererID;
        std::string m_filePath;
        std::unordered_map<std::string, int> m_UniformLocationCache;

        ShaderProgramSource ParseShader() const;
        virtual unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) const = 0;
        virtual unsigned int CompileShader(const std::string& source, const unsigned int type) const = 0;
        virtual int GetUniformLocation(const std::string& name) = 0;

    public:
        Shader(std::string& filePath);
        virtual ~Shader();

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetUniform1i(const std::string& name, const int v1) = 0;

        virtual void SetUniform4f(const std::string& name, const float v1, const float v2, const float v3, const float v4) = 0;

        virtual void SetUniformMat4f(const std::string& name, const Eigen::Matrix4f& matrix) = 0;
    };
}