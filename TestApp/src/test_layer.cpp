#include "test_layer.h"

TestLayer::TestLayer(const std::string& name) : Layer(name) {
    Pagoda::Mirage::VertexBufferLayout layout = Pagoda::Mirage::VertexBufferLayout();
    layout.PushVector3f("POS");
    layout.PushVector4f("COL");

    float triangleArray[] = {
         0.0f,  0.5f, 0.0f,  // point at top
         0.5f, -0.5f, 0.0f,  // point at bottom-right
        -0.5f, -0.5f, 0.0f,  // point at bottom-left
    };

    float squareArray[] = {
       -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // point at top-left        0
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,    // point at top-right       1
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // point at bottom-right    2
       -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f  // point at bottom-left     3
    };

    int indicies[] = {
        0, 1, 2,
        3, 0, 2,
    };

    Pagoda::Mirage::VertexBuffer* buff = Pagoda::Mirage::MirageFactory::CreateVertexBuffer(squareArray, sizeof(squareArray), 4, layout);
    Pagoda::Mirage::IndexBuffer* indexBuff = Pagoda::Mirage::MirageFactory::CreateIndexBuffer(indicies, sizeof(indicies));
    Pagoda::Mirage::Shader* vShader = Pagoda::Mirage::MirageFactory::CreateShader(std::string("C:/Dev/Pagoda/Pagoda/res/mirage/platform/d3d11/shader/dummy.hlsl"), layout, Pagoda::Mirage::ShaderType::SHADER_TYPE_VERTEX);
    Pagoda::Mirage::Shader* pShader = Pagoda::Mirage::MirageFactory::CreateShader(std::string("C:/Dev/Pagoda/Pagoda/res/mirage/platform/d3d11/shader/dummy.hlsl"), layout, Pagoda::Mirage::ShaderType::SHADER_TYPE_FRAGMENT);

    this->m_Model = Pagoda::Mirage::Model({buff}, indexBuff);
    this->m_pipelineState = Pagoda::Mirage::MirageFactory::CreatePipelineState(vShader, pShader, layout);

    this->m_Renderer = Pagoda::Mirage::MirageFactory::CreateRenderer();
}

TestLayer::~TestLayer() {

}

void TestLayer::OnEvent(Pagoda::Base::Event& e) const {
}

void TestLayer::OnUpdate() const {
    this->m_Renderer->Draw(this->m_Model, this->m_pipelineState);
}

void TestLayer::OnAttach() {
}