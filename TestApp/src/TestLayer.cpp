#include "TestLayer.h"

TestLayer::TestLayer(const std::string& name) : Layer(name) {}

TestLayer::~TestLayer() {

}

void TestLayer::OnEvent(Pagoda::Base::Event& e) const {
}

void TestLayer::OnUpdate() const {
    this->m_Renderer->Draw();
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

   Pagoda::Mirage::VertexBuffer* vertexBuffer = Pagoda::Mirage::GLVertexBuffer::Create(vertex, sizeof(vertex));
   Pagoda::Mirage::VertexBufferLayout* layout = Pagoda::Mirage::GLVertexBufferLayout::Create();

   Pagoda::Mirage::IndexBuffer* indexBuffer = Pagoda::Mirage::GLIndexBuffer::Create(indicies, sizeof(indicies));

   layout->PushFloat(2);

   Pagoda::Mirage::Model* model = new Pagoda::Mirage::Model(*vertexBuffer, *layout, *indexBuffer);

   std::string shaderPath("E:/Dev/Pagoda/Pagoda/res/shader/Basic2D.shader");
   Pagoda::Mirage::GLShader* shaderProgram = new Pagoda::Mirage::GLShader(shaderPath);

   this->m_Renderer = new Pagoda::Mirage::GLRenderer(*model, *shaderProgram);
}