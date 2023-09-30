#pragma once
#include "pgpch.h"

#include "mirage/core/buffer/pg_vertex_buffer.h"
#include "mirage/core/buffer/pg_index_buffer.h"

namespace Pagoda::Mirage {
    class Model {
    public:
        Model() : m_IndexBuffer(nullptr), m_IsIndexed(false) {}
        Model(std::vector<VertexBuffer*> vbs, IndexBuffer* ib = nullptr) : m_VertexBuffers(vbs), m_IndexBuffer(ib), m_IsIndexed(true) {}
        virtual ~Model() {}

        void Bind() const {
            for (VertexBuffer* buff : this->m_VertexBuffers) {
                buff->Bind();
            }

            if (m_IsIndexed) {
                this->m_IndexBuffer->Bind();
            }
        }

        inline unsigned int GetVertexCount() const {
            // Return IndexBuffer count if in use, otherwise aggregate the total verticies in each buffer
            if (m_IsIndexed) {
                return this->m_IndexBuffer->GetCount();
            } else {
                unsigned int count = 0;
                for (VertexBuffer* buff : this->m_VertexBuffers) {
                    count += buff->GetVertexCount();
                }

                return count;
            }
        }

    private:
        std::vector<VertexBuffer*> m_VertexBuffers;
        IndexBuffer* m_IndexBuffer;

        bool m_IsIndexed;
    };
}