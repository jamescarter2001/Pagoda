#pragma once
#include "pgpch.h"

#include "mirage/core/model/pg_model.h"
#include "mirage/core/pipeline/pg_pipeline_state.h"

namespace Pagoda::Mirage {
    class Renderer {
    public:
        Renderer();
        virtual ~Renderer();

        virtual void Draw(const Model& model, const PipelineState* pipelineState) const;
    };
}