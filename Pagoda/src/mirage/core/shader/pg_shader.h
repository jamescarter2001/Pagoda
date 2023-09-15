#pragma once
#include "pgpch.h"

namespace Pagoda::Mirage {
    class Shader {
    public:
        Shader() {}
        virtual ~Shader() {}

        virtual void Bind() = 0;
        virtual void Unbind() = 0;
    };

    struct ShaderPair {
        Shader* vertexShader;
        Shader* fragmentShader;
    };
}