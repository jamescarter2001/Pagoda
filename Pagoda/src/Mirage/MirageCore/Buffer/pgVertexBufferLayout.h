#pragma once

#include "pgpch.h"

namespace Pagoda::Mirage {
    struct VertexBufferElement {
        unsigned int type;
        unsigned int count;
        unsigned char normalized;
    };

    class PAGODA_API VertexBufferLayout {
    public:
        static VertexBufferLayout* Create();

        virtual ~VertexBufferLayout() {}
        
        static int GetSizeOfType(unsigned int type);

        virtual void PushFloat(unsigned int count) = 0;
        virtual void PushInt(unsigned int count) = 0;
        virtual void PushChar(unsigned int count) = 0;

        inline unsigned int GetStride() const {
            return m_Stride;
        }

        inline std::vector<VertexBufferElement> GetElements() const& {
            return m_Elements;
        }

    protected:
        template <typename T>
        void Push(unsigned int type, unsigned int count, unsigned char normalized) {
            m_Elements.push_back({type, count, normalized});
            m_Stride += sizeof(T) * count;
        }
        
    private:
        std::vector<VertexBufferElement> m_Elements;
        unsigned int m_Stride;
    };
}
