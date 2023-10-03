#pragma once
#include "pgpch.h"

#include "mirage/core/model/pg_model.h"
#include "mirage/core/pipeline/pg_pipeline_state.h"
#include "mirage/core/buffer/pg_constant_buffer.h"

namespace Pagoda::Mirage {
    class Renderer {
    public:
        Renderer();
        virtual ~Renderer();

        virtual void Draw(const Model& model, const PipelineState* pipelineState) const;

        template<typename T>
        void SetConstantBuffer(const ConstantBuffer<T>* cb) {
            cb->Bind();
        }
    };
}