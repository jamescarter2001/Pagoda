#include "pgpch.h"
#include "pgFXPipeline.h"

namespace Pagoda::Mirage {
    FXPipeline::FXPipeline() {
        std::string shaderPath("E:/Dev/Pagoda/Pagoda/res/shader/Basic2D.shader");
        Shader* shaderProgram = MiragePipelineFactory::CreateShader(shaderPath);
        this->m_Shader = shaderProgram;
    }

    FXPipeline::~FXPipeline() {
        delete this->m_Shader;
    }

    void FXPipeline::Run() const {
        this->Clear();

        for (Model* m : this->m_Models) {
            Renderer* renderer = MiragePipelineFactory::CreateRenderer(*m, *this->m_Shader);
            renderer->Draw();
            delete renderer;
        }
    }

    void FXPipeline::Register(Model* m) {
        this->m_Models.push_back(m);
    }

    void FXPipeline::Unregister(Model* m) {
        // TODO - Probably need to error check this.
        this->m_Models.erase(std::find(m_Models.begin(), m_Models.end(), m));
    }
}