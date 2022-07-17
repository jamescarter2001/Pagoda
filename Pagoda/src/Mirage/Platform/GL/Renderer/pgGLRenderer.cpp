#include "pgpch.h"
#include "pgGLRenderer.h"

namespace Pagoda::Mirage {
    GLRenderer::GLRenderer(Model& model, GLShader& shader) : Renderer(model), m_Shader(shader) {

        this->m_Vao = new GLVertexArray();

        this->m_Vao->Bind();
        this->m_Vao->AddBuffer(this->m_Model.GetVertexBuffer(), this->m_Model.GetVertexBufferLayout());
    }

    GLRenderer::~GLRenderer() {
        delete m_Vao;
    }

    void GLRenderer::Clear() const {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void GLRenderer::Draw() const {
        this->Clear();

        this->m_Model.Bind();
        this->m_Shader.Bind();
        glDrawElements(GL_TRIANGLES, this->m_Model.GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr);
    }
}