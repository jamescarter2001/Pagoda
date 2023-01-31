#include "pgpch.h"
#include "pgFXPipeline.h"

namespace Pagoda::Mirage {
    FXPipeline::FXPipeline() {
        std::string shaderPath("E:/Dev/Pagoda/Pagoda/res/shader/Basic2D.shader");
        Shader* shaderProgram = MirageFactory::CreateShader(shaderPath);
        this->m_Shader = shaderProgram;
    }

    FXPipeline::~FXPipeline() {
        delete this->m_Shader;
    }

    void FXPipeline::Run() const {
        this->Clear();

        for (Renderer* r : this->m_Renderers) {
            r->Draw();
        }
    }

    void FXPipeline::Register(Model* m) {
        Renderer* renderer = MirageFactory::CreateRenderer(*m, *this->m_Shader);
        this->m_Renderers.push_back(renderer);
        this->m_ModelRenderers[m] = renderer;
    }

    void FXPipeline::Unregister(Model* m) {
        // TODO - Probably need to error check this.
        Renderer* renderer = m_ModelRenderers[m];

        std::vector<Renderer*>::iterator it = std::find(m_Renderers.begin(), m_Renderers.end(), renderer);
        this->m_Renderers.erase(it);

        this->m_ModelRenderers.erase(m);
        delete renderer;
    }
}