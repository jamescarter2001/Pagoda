#pragma once

#include "pgpch.h"

namespace Pagoda::Mirage {
    struct VertexBufferElement {
        unsigned int size;
        unsigned int count;
        unsigned char normalized; // OpenGL requires this
    };

    class PAGODA_API VertexBufferLayout {
    public:
        VertexBufferLayout() {}
        virtual ~VertexBufferLayout() {}

        static int GetSizeOfType(unsigned int type) {
            switch (type) {
                case PG_FLOAT:
                    return sizeof(float);
                case PG_UNSIGNED_INT:
                    return sizeof(unsigned int);
                case PG_UNSIGNED_BYTE:
                    return sizeof(unsigned char);
                default:
                    return sizeof(float);
            }
        }

        inline unsigned int GetStride() const {
            return m_Stride;
        }

        inline std::vector<VertexBufferElement> GetElements() const& {
            return m_Elements;
        }

        template <typename T>
        void Push(unsigned int count) {
            m_Elements.push_back({sizeof(T), count});
            m_Stride += sizeof(T) * count;
        }

    private:
        std::vector<VertexBufferElement> m_Elements;
        unsigned int m_Stride = 0;
    };
}