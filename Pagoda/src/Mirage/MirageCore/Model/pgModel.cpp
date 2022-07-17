#include "pgpch.h"

#include "pgModel.h"

namespace Pagoda::Mirage {
    Model::Model(VertexBuffer& vb, VertexBufferLayout& vbl, IndexBuffer& ib) : m_VertexBuffer(vb), m_VertexBufferLayout(vbl), m_IndexBuffer(ib) {}

    Model::~Model() {}

    void Model::Bind() const {
        this->m_VertexBuffer.Bind();
        this->m_IndexBuffer.Bind();
    }

    void Model::Unbind() const {
        this->m_VertexBuffer.Unbind();
        this->m_IndexBuffer.Unbind();
    }
}