#pragma once

#include "pgpch.h"

#include "Mirage/pgMirageFactory.h"

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
        std::unordered_map<Model*, Renderer*> m_ModelRenderers;
        std::vector<Renderer*> m_Renderers;
        Shader* m_Shader;
    };
}