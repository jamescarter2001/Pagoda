#pragma once
#include "pgpch.h"

#include "mirage/core/buffer/pg_vertex_buffer.h"

namespace Pagoda::Mirage {
    class Model {
    public:
        Model() {}
        virtual ~Model() {}

        void Bind() {
            for (VertexBuffer* buff : this->m_VertexBuffers) {
                buff->Bind();
            }
        }

        inline unsigned int GetVertexCount() {
            unsigned int count = 0;
            for (VertexBuffer* buff : this->m_VertexBuffers) {
                count += buff->GetCount();
            }

            return count;
        }

    private:
        std::vector<VertexBuffer*> m_VertexBuffers;
    };
}