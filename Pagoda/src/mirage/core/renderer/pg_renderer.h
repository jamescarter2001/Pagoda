#pragma once
#include "pgpch.h"

#include "mirage/core/model/pg_model.h"
#include "mirage/core/shader/pg_shader.h"

namespace Pagoda::Mirage {
    class Renderer {
    public:
        Renderer();
        virtual ~Renderer();

        virtual void Draw(Model& model, Shader& shader);
    };
}