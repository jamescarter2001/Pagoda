#include "pgpch.h"

#include "pgModel.h"

namespace Pagoda::Mirage {
    Model::Model(VertexBuffer& vb, IndexBuffer& ib) : m_VertexBuffer(vb), m_IndexBuffer(ib) {}

    Model::~Model() {}
}