#pragma once

#include "pgpch.h"

namespace Pagoda::Mirage {
    struct VertexBufferElement {
        unsigned int type;
        unsigned int count;
        unsigned char normalized;
    };

    class VertexBufferLayout {
    public:
        static int GetSizeOfType(unsigned int type);

        template <typename T>
        void Push(unsigned int count) {
            static_assert(false);
        }

        inline unsigned int GetStride() {
            return m_Stride;
        }

        inline std::vector<VertexBufferElement> GetElements() const& {
            return m_Elements;
        }

    protected:
        template <typename T>
        void PushElement(unsigned int type, unsigned int count, unsigned char normalized) {
            m_Elements.push_back({type, count, normalized});
            m_Stride += sizeof(T) * count;
        }
        
    private:
        std::vector<VertexBufferElement> m_Elements;
        unsigned int m_Stride;
    };
}