#pragma once

#include "pgpch.h"

#include <glad/glad.h>

#include "Mirage/MirageCore/Buffer/pgVertexBufferLayout.h"

namespace Pagoda::Mirage {
    class GLVertexBufferLayout : public VertexBufferLayout {
    public:
        static int GetSizeOfType(unsigned int type) {
            switch (type) {
                case GL_FLOAT:
                    return sizeof(float);
                case GL_UNSIGNED_INT:
                    return sizeof(unsigned int);
                case GL_UNSIGNED_BYTE:
                    return sizeof(unsigned char);
                default:
                    return sizeof(float);
            }
        }

        template <typename T>
        void Push(unsigned int count) {
            static_assert(false);
        }
    };
    // Explicit template specialisation must be declared outside of class scope.
    template <>
    void GLVertexBufferLayout::Push<float>(unsigned int count) {
        this->PushElement<float>(GL_FLOAT, count, GL_FALSE);
    }

    template <>
    void GLVertexBufferLayout::Push<unsigned int>(unsigned int count) {
        this->PushElement<unsigned int>(GL_UNSIGNED_INT, count, GL_TRUE);
    }

    template <>
    void GLVertexBufferLayout::Push<unsigned char>(unsigned int count) {
        this->PushElement<unsigned char>(GL_UNSIGNED_BYTE, count, GL_TRUE);
    }
}