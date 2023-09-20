#pragma once

#include "pgpch.h"

namespace Pagoda::Mirage {
    struct VertexBufferElement {
        std::string name;
        unsigned int size;
        unsigned int count;
        unsigned char normalized; // OpenGL requires this
        unsigned int platformFormat; // Platform specific format eg. DXGI_FORMAT_R32G32B32_FLOAT
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

        inline unsigned int GetCount() const {
            return (unsigned int) this->m_Elements.size();
        }

        inline unsigned int GetStride() const {
            return m_Stride;
        }

        inline const unsigned int* GetStridePtr() const {
            return &m_Stride;
        }

        inline std::vector<VertexBufferElement> GetElements() const& {
            return m_Elements;
        }

        /* template <typename T>
        inline void Push(unsigned int count) {
            m_Elements.push_back({sizeof(T), count});
            m_Stride += sizeof(T) * count;

            if ((sizeof(T) * count) == 16) {

            }
        }*/

        inline void PushVector3f(std::string name) {
            m_Elements.push_back({name, sizeof(float), 3, 0, PG_VECTOR3});
            m_Stride += sizeof(float) * 3;
        }

        inline void PushVector4f(std::string name) {
            m_Elements.push_back({name, sizeof(float), 4, 0, PG_VECTOR4});
            m_Stride += sizeof(float) * 4;
        }

    private:
        std::vector<VertexBufferElement> m_Elements;
        unsigned int m_Stride = 0;
    };
}