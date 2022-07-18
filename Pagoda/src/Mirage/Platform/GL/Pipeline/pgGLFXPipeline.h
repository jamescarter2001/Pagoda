#pragma once

#include "pgpch.h"

#include "Mirage/MirageCore/Pipeline/pgFXPipeline.h"

namespace Pagoda::Mirage {
    class GLFXPipeline : public FXPipeline {
    public:
        GLFXPipeline();
        virtual ~GLFXPipeline();
    private:
        virtual void Clear() const override;
    };
}