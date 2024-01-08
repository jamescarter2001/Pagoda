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

    float cubeArray[] = {
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
       -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f
    };

    unsigned int Indices[] = {
        0, 1, 2,
        1, 3, 4,
        5, 6, 3,
        7, 3, 6,
        2, 4, 7,
        0, 7, 6,
        0, 5, 1,
        1, 5, 3,
        5, 0, 6,
        7, 4, 3,
        2, 1, 4,
        0, 2, 7};

    m_translation = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 1.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 1.0f, 0.0f,
                              0.0f, 0.0f, 2.0f, 1.0f);

    m_identity = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, 1.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 1.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 1.0f);

    Pagoda::Mirage::VertexBuffer* buff = Pagoda::Mirage::MirageFactory::CreateVertexBuffer(cubeArray, sizeof(cubeArray), 8, layout);
    Pagoda::Mirage::IndexBuffer* indexBuff = Pagoda::Mirage::MirageFactory::CreateIndexBuffer(Indices, sizeof(Indices));
    Pagoda::Mirage::Shader* vShader = Pagoda::Mirage::MirageFactory::CreateShader(std::string("../Pagoda/res/mirage/platform/d3d11/shader/dummy.hlsl"), layout, Pagoda::Mirage::ShaderType::SHADER_TYPE_VERTEX);
    Pagoda::Mirage::Shader* pShader = Pagoda::Mirage::MirageFactory::CreateShader(std::string("../Pagoda/res/mirage/platform/d3d11/shader/dummy.hlsl"), layout, Pagoda::Mirage::ShaderType::SHADER_TYPE_FRAGMENT);

    m_constantBuffer = Pagoda::Mirage::MirageFactory::CreateTransformConstantBuffer(sizeof(m_identity));

    this->m_Model = Pagoda::Mirage::Model({buff}, indexBuff);
    this->m_pipelineState = Pagoda::Mirage::MirageFactory::CreatePipelineState(vShader, pShader, layout);

    this->m_Renderer = Pagoda::Mirage::MirageFactory::CreateRenderer();
}

TestLayer::~TestLayer() {

}

void TestLayer::OnEvent(Pagoda::Base::Event& e) const {
}

void TestLayer::OnUpdate() {
    this->m_scale = m_scale + 0.005f;
    glm::mat4 rot(glm::cos(m_scale), 0.0f, -glm::sin(m_scale), 0.0f,
                  0.0f, 1.0f, 0.0f, 0.0f,
                  glm::sin(m_scale), 0.0f, glm::cos(m_scale), 0.0f,
                  0.0f, 0.0f, 0.0f, 1.0f);

    glm::mat4 fin = m_translation * rot * m_identity;

    

    this->m_constantBuffer->Write(&fin[0][0]);
    this->m_Renderer->Draw(this->m_Model, this->m_pipelineState, this->m_constantBuffer, true);
}

void TestLayer::OnAttach() {
}