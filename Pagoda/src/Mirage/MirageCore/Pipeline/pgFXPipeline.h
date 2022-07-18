#pragma once

#include "pgpch.h"

#include "Mirage/pgMiragePipelineFactory.h"

namespace Pagoda::Mirage {
    class FXPipeline {
    public:
        FXPipeline();
        virtual ~FXPipeline();

        void Run() const;
        void Register(Model* o);
        void Unregister(Model* o);

        virtual void Clear() const = 0;

    private:
        std::vector<Model*> m_Models;
        Shader* m_Shader;
    };
}