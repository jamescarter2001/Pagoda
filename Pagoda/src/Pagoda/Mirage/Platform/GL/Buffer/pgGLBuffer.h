#pragma once
#include "pgpch.h"

#include <glad/glad.h>

#include "Mirage/MirageCore/Buffer/pgBuffer.h"

namespace Pagoda::Mirage {
    template <typename T>
    class GLBuffer : public Buffer {
    public:
        virtual void Bind() const override {
            glBindBuffer(m_Type, m_RendererID);
        }

        virtual void Unbind() const override {
            glBindBuffer(m_Type, 0);
        }

    protected:
        GLBuffer(const T data, const unsigned int size, unsigned int type) : m_Type(type) {
            glGenBuffers(1, &m_RendererID);
            glBindBuffer(m_Type, m_RendererID);
            glBufferData(m_Type, size, data, GL_STATIC_DRAW);
        }

        ~GLBuffer() {
            glDeleteBuffers(1, &m_RendererID);
        }

    private:
        unsigned int m_Type;
    };
}