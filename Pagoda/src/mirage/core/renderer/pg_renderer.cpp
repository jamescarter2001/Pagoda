#include "pgpch.h"
#include "pg_renderer.h"

namespace Pagoda::Mirage {
    Renderer::Renderer() {
        // TODO: Extract to Camera class.
        this->m_mvpMatrixBuffer = Base::Lazy<ConstantBuffer<float>>::val([this]() { return this->CreateMVP(); });

        float fov = 90.0f;
        float tanHalfFov = glm::tan(glm::radians(fov / 2.0f));
        float f = 1 / tanHalfFov;

        // TODO: Read active window data.
        float aspectRatio = 1280.0f / 720.0f;

        this->m_mvpMatrix = glm::mat4(f / aspectRatio, 0.0f, 0.0f, 0.0f,
                                      0.0f, f, 0.0f, 0.0f,
                                      0.0f, 0.0f, 1.0f, 1.0f,
                                      0.0f, 0.0f, 0.0f, 0.0f);
    }

    Renderer::~Renderer() {}

    RendererAPI Renderer::GetRendererAPI() {
        return RendererAPI::Direct3D12;
    }

    void Renderer::Draw(const Model& model, const PipelineState* pipelineState, bool project) {
        this->Draw(model, pipelineState, nullptr, project);
    }

    void Renderer::Draw(const Model& model, const PipelineState* pipelineState, const ConstantBuffer<float>* transform, bool project) {

        // Transform if specified.
        if (transform != nullptr) {
            transform->Bind();
        }

        // Project for 3D if required.
        if (project) {
            this->m_mvpMatrixBuffer.Get()->Bind();
        }

        // Bind the pipeline state.
        pipelineState->Bind();

        // Bind the model data.
        model.Bind();

        // glm::vec4 test = this->m_mvpMatrix * *(glm::mat4*) transform->GetBuffer() * glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f);
        // PG_CORE_DEBUG("{}, {}, {}, {}", test[0], test[1], test[2], test[3]);
    }
}