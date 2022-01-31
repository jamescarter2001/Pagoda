#pragma once
#include "pgpch.h"

#include <glad/glad.h>

#include "Mirage/MirageCore/Buffer/pgBuffer.h"

template<typename T> class OpenGLBuffer : public Buffer {
	OpenGLBuffer(const T data, const unsigned int size, unsigned int type) : m_Type(type) {
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(m_Type, m_RendererID);
		glBufferData(m_Type, size, data, GL_STATIC_DRAW);
	}

	~OpenGLBuffer() {
		glDeleteBuffers(1, &m_RendererID);
	}

public:

	void Bind() const override {
		glBindBuffer(m_Type, m_RendererID);
	}

	void Unbind() const override {
		glBindBuffer(m_Type, 0);
	}
};