#pragma once
#include "pgpch.h"

#include "mirage/core/window/pg_window_data.h"
#include "mirage/core/model/pg_model.h"
#include "mirage/core/pipeline/pg_pipeline_state.h"
#include "mirage/core/buffer/pg_constant_buffer.h"

namespace Pagoda::Mirage {
    class Renderer {
    public:
        Renderer(WindowData* wd);
        virtual ~Renderer();

        virtual void Draw(const Model& model, const PipelineState* pipelineState, bool project);
        virtual void Draw(const Model& model, const PipelineState* pipelineState, const ConstantBuffer<float>* transform, bool project);
        virtual ConstantBuffer<float>* CreateMVP() = 0;

        template<typename T>
        void UseConstantBuffer(const ConstantBuffer<T>* cb) {
            cb->Bind();
        }

    protected:
        glm::mat4 m_mvpMatrix;

    private:
        WindowData* m_windowData;
        ConstantBuffer<float>* m_mvpMatrixBuffer;
    };
}