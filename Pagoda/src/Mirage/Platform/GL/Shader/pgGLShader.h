#pragma once

#include "pgpch.h"

#include <glad/glad.h>

#include "Mirage/MirageCore/Shader/pgShader.h"

namespace Pagoda::Mirage {
    class GLShader : public Shader {
    protected:
        virtual unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) const override;
        virtual unsigned int CompileShader(const std::string& source, const unsigned int type) const override;
        virtual int GetUniformLocation(const std::string& name) override;

    public:
        GLShader(std::string& filePath);
        virtual ~GLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void SetUniform1i(const std::string& name, const int v1) override;

        virtual void SetUniform4f(const std::string& name, const float v1, const float v2, const float v3, const float v4) override;

        virtual void SetUniformMat4f(const std::string& name, const Eigen::Matrix4f& matrix) override;
    };
}