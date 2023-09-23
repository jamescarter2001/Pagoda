#include "pgpch.h"

#include "pg_gl_vertex_buffer_layout.h"

namespace Pagoda::Mirage {
    VertexBufferLayout* VertexBufferLayout::Create() {
        return new GLVertexBufferLayout();
    }

    GLVertexBufferLayout::~GLVertexBufferLayout() {}

    int VertexBufferLayout::GetSizeOfType(unsigned int type) {
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

    void GLVertexBufferLayout::PushFloat(unsigned int count) {
        this->Push<float>(GL_FLOAT, count, GL_FALSE);
    }

    void GLVertexBufferLayout::PushInt(unsigned int count) {
        this->Push<unsigned int>(GL_UNSIGNED_INT, count, GL_TRUE);
    }

    void GLVertexBufferLayout::PushChar(unsigned int count) {
        this->Push<unsigned char>(GL_UNSIGNED_BYTE, count, GL_TRUE);
    }
}