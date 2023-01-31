#include "TestLayer.h"

using namespace Pagoda::Mirage;

TestLayer::TestLayer(const std::string& name) : Layer(name) {}

TestLayer::~TestLayer() {

}

void TestLayer::OnEvent(Pagoda::Base::Event& e) const {
}

void TestLayer::OnUpdate() const {
    this->m_FXPipeline->Run();
}

void TestLayer::OnAttach() {
    float vertex[] = {
       -0.5f,-0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,
       -0.5f,0.5f,
    };

   unsigned int indicies[] = {
        0, 1, 2,
        2, 3, 0
   };

   Pagoda::Mirage::VertexBuffer* vertexBuffer = MirageFactory::CreateVertexBuffer(vertex, sizeof(vertex));
   Pagoda::Mirage::VertexBufferLayout* layout = MirageFactory::CreateVertexBufferLayout();

   Pagoda::Mirage::IndexBuffer* indexBuffer = MirageFactory::CreateIndexBuffer(indicies, sizeof(indicies));

   layout->PushFloat(2);

   Pagoda::Mirage::Model* model = new Pagoda::Mirage::Model(*vertexBuffer, *layout, *indexBuffer);

   std::string shaderPath("E:/Dev/Pagoda/Pagoda/res/shader/Basic2D.shader");
   Pagoda::Mirage::GLShader* shaderProgram = new Pagoda::Mirage::GLShader(shaderPath);

   this->m_FXPipeline = new Pagoda::Mirage::GLFXPipeline();
   this->m_FXPipeline->Register(model);
}