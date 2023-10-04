#include "pgpch.h"
#include "pg_renderer.h"

namespace Pagoda::Mirage {
    Renderer::Renderer(WindowData* wd) {
        this->m_windowData = wd;
        this->m_mvpMatrixBuffer = nullptr;

        float fov = 90.0f;
        float tanHalfFov = glm::tan(glm::radians(fov / 2.0f));
        float f = 1 / tanHalfFov;

        float aspectRatio = (float) this->m_windowData->Width / (float) this->m_windowData->Height;

        this->m_mvpMatrix = glm::mat4(f / aspectRatio, 0.0f, 0.0f, 0.0f,
                                      0.0f, f, 0.0f, 0.0f,
                                      0.0f, 0.0f, 1.0f, 1.0f,
                                      0.0f, 0.0f, 0.0f, 0.0f);
    }

    Renderer::~Renderer() {}

    void Renderer::Draw(const Model& model, const PipelineState* pipelineState, bool project) {
        this->Draw(model, pipelineState, nullptr, project);
    }

    void Renderer::Draw(const Model& model, const PipelineState* pipelineState, const ConstantBuffer<float>* transform, bool project) {
        if (this->m_mvpMatrixBuffer == nullptr) {
            this->m_mvpMatrixBuffer = this->CreateMVP();
        }

        // Project for 3D if required.
        if (project) {
            this->m_mvpMatrixBuffer->Bind();
        }

        // Transform if specified.
        if (transform != nullptr) {
            transform->Bind();
        }

        pipelineState->Bind();
        model.Bind();

        glm::vec4 test = this->m_mvpMatrix * *(glm::mat4*) transform->GetBuffer() * glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f);
        PG_CORE_DEBUG("{}, {}, {}, {}", test[0], test[1], test[2], test[3]);
    }
}