#pragma once
#include "pgpch.h"

#include "mirage/core/window/pg_window_data.h"
#include "mirage/core/model/pg_model.h"
#include "mirage/core/pipeline/pg_pipeline_state.h"
#include "mirage/core/buffer/pg_constant_buffer.h"

namespace Pagoda::Mirage {
    enum RendererAPI {
        NONE = 0,
        Direct3D11 = 1,
        Direct3D12 = 2
    };

    class Renderer {
    public:
        Renderer();
        virtual ~Renderer();

        static RendererAPI GetRendererAPI();

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
        ConstantBuffer<float>* m_mvpMatrixBuffer;
    };
}