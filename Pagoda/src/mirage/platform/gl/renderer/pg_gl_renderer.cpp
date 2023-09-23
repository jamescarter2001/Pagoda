#include "pgpch.h"
#include "pg_gl_renderer.h"

namespace Pagoda::Mirage {
    GLRenderer::GLRenderer() {

        this->m_Vao = new GLVertexArray();

        this->m_Vao->Bind();
        this->m_Vao->AddBuffer(this->m_Model.GetVertexBuffer(), this->m_Model.GetVertexBufferLayout());
    }

    GLRenderer::~GLRenderer() {
        delete m_Vao;
    }

    void GLRenderer::Draw(const Model& model, const ShaderData& shader) const {
        this->m_Model.Bind();
        this->m_Shader.Bind();
        glDrawElements(GL_TRIANGLES, this->m_Model.GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr);
    }
}