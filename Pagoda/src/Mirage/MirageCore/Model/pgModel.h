#pragma once
#include "pgpch.h"

#include "Mirage/MirageCore/Buffer/pgVertexBuffer.h"
#include "Mirage/MirageCore/Buffer/pgIndexBuffer.h"

namespace Pagoda::Mirage {
    class Model {
    public:
        Model(VertexBuffer& vb, IndexBuffer& ib);
        virtual ~Model();

        void Bind() const;
        void Unbind() const;
    private:
        VertexBuffer& m_VertexBuffer;
        IndexBuffer& m_IndexBuffer;
    };
}