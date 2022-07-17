#include "pgpch.h"
#include "pgGLRenderer.h"

namespace Pagoda::Mirage {
    GLRenderer::GLRenderer() {}
    GLRenderer::~GLRenderer() {}

    void GLRenderer::Clear() const {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void GLRenderer::Draw(Model& model) const {
        GLVertexArray vao = GLVertexArray();
        vao.Bind();

        vao.AddBuffer(model.GetVertexBuffer(), model.GetVertexBufferLayout());
    }
}