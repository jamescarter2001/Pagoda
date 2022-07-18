#include "pgpch.h"

#include "pgShader.h"

namespace Pagoda::Mirage {
    Shader::Shader(std::string& filePath) : m_filePath(filePath) {}

    Shader::~Shader() {}

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
                } else if (line.find("fragment") != std::string::npos) {
                    type = ShaderType::FRAGMENT;
                }
            } else {
                if (type == ShaderType::VERTEX) {
                    vertexShader << line << std::endl;
                } else if (type == ShaderType::FRAGMENT) {
                    fragmentShader << line << std::endl;
                }
            }
        }

        ShaderProgramSource shaderPair;
        shaderPair.vertexShader = vertexShader.str();
        shaderPair.fragmentShader = fragmentShader.str();

        return shaderPair;
    }
}