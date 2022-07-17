#include "pgpch.h"

#include "pgMiragePipelineFactory.h"

namespace Pagoda::Mirage {
    Window* MiragePipelineFactory::CreateWindow(const WindowProps& props) {
        return GLWindow::Create(props);
    }

    VertexBuffer* MiragePipelineFactory::CreateVertexBuffer(void* data, const unsigned int size) {
        return GLVertexBuffer::Create(data, size);
    }

    VertexBufferLayout* MiragePipelineFactory::CreateVertexBufferLayout() {
        return GLVertexBufferLayout::Create();
    }

    IndexBuffer* MiragePipelineFactory::CreateIndexBuffer(unsigned int* data, const unsigned int size) {
        return GLIndexBuffer::Create(data, size);
    }

    Renderer* MiragePipelineFactory::CreateRenderer(Model& model, GLShader& shader) {
        return new GLRenderer(model, shader);
    }
}