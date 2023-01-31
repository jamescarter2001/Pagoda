#include "pgpch.h"

#include "pgMirageFactory.h"

namespace Pagoda::Mirage {
    Window* MirageFactory::CreateWindow(const WindowProps& props) {
        return GLWindow::Create(props);
    }

    VertexBuffer* MirageFactory::CreateVertexBuffer(void* data, const unsigned int size) {
        return GLVertexBuffer::Create(data, size);
    }

    VertexBufferLayout* MirageFactory::CreateVertexBufferLayout() {
        return GLVertexBufferLayout::Create();
    }

    IndexBuffer* MirageFactory::CreateIndexBuffer(unsigned int* data, const unsigned int size) {
        return GLIndexBuffer::Create(data, size);
    }

    Renderer* MirageFactory::CreateRenderer(Model& model, Shader& shader) {
        return new GLRenderer(model, shader);
    }

    Shader* MirageFactory::CreateShader(std::string& filePath) {
        return new GLShader(filePath);
    }
}