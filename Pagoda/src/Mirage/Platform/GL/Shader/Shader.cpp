#include "Shader.h"

Shader::Shader(std::string& filePath) : m_filePath(filePath) {
    ShaderProgramSource source = this->ParseShader();
    m_RendererID = CreateShader(source.vertexShader, source.fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const {
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

ShaderProgramSource Shader::ParseShader() const {
    std::ifstream stream(m_filePath);
    std::stringstream vertexShader;
    std::stringstream fragmentShader;

    ShaderType type = ShaderType::NONE;

    std::string line;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            if (type == ShaderType::VERTEX) {
                vertexShader << line << std::endl;
            }
            else if (type == ShaderType::FRAGMENT) {
                fragmentShader << line << std::endl;
            }
        }
    }

    ShaderProgramSource shaderPair;
    shaderPair.vertexShader = vertexShader.str();
    shaderPair.fragmentShader = fragmentShader.str();

    return shaderPair;
}

unsigned int Shader::CompileShader(const std::string& source, const unsigned int type) const {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        std::cout << "Shader Compilation Error [" << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << "]" << std::endl;

        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) const {
    unsigned int program = glCreateProgram();

    unsigned int vs = this->CompileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = this->CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    return program;
}

int Shader::GetUniformLocation(const std::string& name) {
    this->Bind();
    if (!m_UniformLocationCache[name]) {
        int location = glGetUniformLocation(m_RendererID, name.c_str());
        PG_ASSERT(location != -1);
        m_UniformLocationCache[name] = location;

        return location;
    }

    return m_UniformLocationCache[name];
}

void Shader::SetUniform1i(const std::string& name, const int v1) {
    glUniform1i(this->GetUniformLocation(name), v1);
}

void Shader::SetUniform4f(const std::string& name, const float v1, const float v2, const float v3, const float v4) {
   glUniform4f(this->GetUniformLocation(name), v1, v2, v3, v4);
}

void Shader::SetUniformMat4f(const std::string& name, const Eigen::Matrix4f& matrix) {
    glUniformMatrix4fv(this->GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}
