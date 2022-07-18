#include "pgpch.h"

#include "pgGLShader.h"

namespace Pagoda::Mirage {
    GLShader::GLShader(std::string& filePath) : Shader(filePath) {
        ShaderProgramSource source = this->ParseShader();
        m_RendererID = CreateShader(source.vertexShader, source.fragmentShader);
    }

    GLShader::~GLShader() {
        glDeleteProgram(m_RendererID);
    }

    void GLShader::Bind() const {
        glUseProgram(m_RendererID);
    }

    void GLShader::Unbind() const {
        glUseProgram(0);
    }

    unsigned int GLShader::CompileShader(const std::string& source, const unsigned int type) const {
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();

        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);

        if (result == GL_FALSE) {
            //std::cout << "Shader Compilation Error [" << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << "]" << std::endl;
            PG_CORE_ERROR("OpenGL Shader compilation error: ({0})", type == GL_VERTEX_SHADER ? "Vertex" : "Fragment");

            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);

            std::string errorMessage(message);
            errorMessage.pop_back();

            PG_CORE_ERROR(errorMessage);
            glDeleteShader(id);
            return 0;
        }

        return id;
    }

    unsigned int GLShader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) const {
        unsigned int program = glCreateProgram();

        unsigned int vs = this->CompileShader(vertexShader, GL_VERTEX_SHADER);
        unsigned int fs = this->CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

        glAttachShader(program, vs);
        glAttachShader(program, fs);

        glLinkProgram(program);
        glValidateProgram(program);

        return program;
    }

    int GLShader::GetUniformLocation(const std::string& name) {
        this->Bind();
        if (!m_UniformLocationCache[name]) {
            int location = glGetUniformLocation(m_RendererID, name.c_str());
            PG_CORE_ASSERT(location != -1, "Unable to locate uniform")
            m_UniformLocationCache[name] = location;

            return location;
        }

        return m_UniformLocationCache[name];
    }

    void GLShader::SetUniform1i(const std::string& name, const int v1) {
        glUniform1i(this->GetUniformLocation(name), v1);
    }

    void GLShader::SetUniform4f(const std::string& name, const float v1, const float v2, const float v3, const float v4) {
        glUniform4f(this->GetUniformLocation(name), v1, v2, v3, v4);
    }

    void GLShader::SetUniformMat4f(const std::string& name, const Eigen::Matrix4f& matrix) {
        glUniformMatrix4fv(this->GetUniformLocation(name), 1, GL_FALSE, matrix.data());
    }
}