#pragma once
#include "pgpch.h"

#include "Mirage/MirageCore/Buffer/pgVertexBuffer.h"
#include "Mirage/MirageCore/Buffer/pgVertexBufferLayout.h"
#include "Mirage/MirageCore/Buffer/pgIndexBuffer.h"

namespace Pagoda::Mirage {
    class Model {
    public:
        Model(VertexBuffer& vb, VertexBufferLayout& vbl, IndexBuffer& ib);
        virtual ~Model();

        void Bind() const;
        void Unbind() const;

        inline VertexBuffer& GetVertexBuffer() { return this->m_VertexBuffer; };
        inline VertexBufferLayout& GetVertexBufferLayout() { return this->m_VertexBufferLayout; };
        inline IndexBuffer& getIndexBuffer() { return this->m_IndexBuffer; };
    private:
        VertexBuffer& m_VertexBuffer;
        VertexBufferLayout& m_VertexBufferLayout;

        IndexBuffer& m_IndexBuffer;
    };
}