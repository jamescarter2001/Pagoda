#pragma once

#include "pgpch.h"

#include "Mirage/MirageCore/Window/pgWindow.h"
#include "Mirage/Platform/GL/Window/pgGLWindow.h"

#include "Mirage/MirageCore/Buffer/pgVertexBuffer.h"
#include "Mirage/Platform/GL/Buffer/pgGLVertexBuffer.h"

#include "Mirage/MirageCore/Buffer/pgVertexBufferLayout.h"
#include "Mirage/Platform/GL/Buffer/pgGLVertexBufferLayout.h"

#include "Mirage/MirageCore/Buffer/pgIndexBuffer.h"
#include "Mirage/Platform/GL/Buffer/pgGLIndexBuffer.h"

#include "Mirage/MirageCore/Renderer/pgRenderer.h"
#include "Mirage/Platform/GL/Renderer/pgGLRenderer.h"

#include "Mirage/MirageCore/Shader/pgShader.h"
#include "Mirage/Platform/GL/Shader/pgGLShader.h"

namespace Pagoda::Mirage {
    class MirageFactory {
    public:
        static Window* CreateWindow(const WindowProps& props);

        static VertexBuffer* CreateVertexBuffer(void* data, const unsigned int size);
        static VertexBufferLayout* CreateVertexBufferLayout();
        static IndexBuffer* CreateIndexBuffer(unsigned int* data, const unsigned int size);

        static Shader* CreateShader(std::string& filePath);

        static Renderer* CreateRenderer(Model& model, Shader& shader);
    };
};