#include "pgpch.h"
#include "pgGLFXPipeline.h"

namespace Pagoda::Mirage {
    GLFXPipeline::GLFXPipeline() : FXPipeline() {}

    GLFXPipeline::~GLFXPipeline() {}

    void GLFXPipeline::Clear() const {
        glClear(GL_COLOR_BUFFER_BIT);
    }
}