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

    float fov = 90.0f;
    float tanHalfFov = glm::tan(glm::radians(fov / 2.0f));
    float f = 1 / tanHalfFov;

    m_translation = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 1.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 1.0f, 3.0f,
                              0.0f, 0.0f, 0.0f, 1.0f);

    m_projection = glm::mat4(f,    0.0f, 0.0f, 0.0f,
                             0.0f, f   , 0.0f, 0.0f,
                             0.0f, 0.0f, 1.0f, 0.0f,
                             0.0f, 0.0f, 1.0f, 0.0f);

    m_identity = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, 1.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 1.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 1.0f);

    Pagoda::Mirage::VertexBuffer* buff = Pagoda::Mirage::MirageFactory::CreateVertexBuffer(squareArray, sizeof(squareArray), 4, layout);
    Pagoda::Mirage::IndexBuffer* indexBuff = Pagoda::Mirage::MirageFactory::CreateIndexBuffer(indicies, sizeof(indicies));
    Pagoda::Mirage::Shader* vShader = Pagoda::Mirage::MirageFactory::CreateShader(std::string("C:/Dev/Pagoda/Pagoda/res/mirage/platform/d3d11/shader/dummy.hlsl"), layout, Pagoda::Mirage::ShaderType::SHADER_TYPE_VERTEX);
    Pagoda::Mirage::Shader* pShader = Pagoda::Mirage::MirageFactory::CreateShader(std::string("C:/Dev/Pagoda/Pagoda/res/mirage/platform/d3d11/shader/dummy.hlsl"), layout, Pagoda::Mirage::ShaderType::SHADER_TYPE_FRAGMENT);

    m_constantBuffer = Pagoda::Mirage::MirageFactory::CreateConstantBuffer<float>(&m_identity[0][0], sizeof(m_identity));

    this->m_Model = Pagoda::Mirage::Model({buff}, indexBuff);
    this->m_pipelineState = Pagoda::Mirage::MirageFactory::CreatePipelineState(vShader, pShader, layout);

    this->m_Renderer = Pagoda::Mirage::MirageFactory::CreateRenderer();
}

TestLayer::~TestLayer() {

}

void TestLayer::OnEvent(Pagoda::Base::Event& e) const {
}

void TestLayer::OnUpdate() const {
    glm::mat4 rot(glm::cos(m_scale), 0.0f, -glm::cos(m_scale), 0.0f,
                  0.0f, 1.0f, 0.0f, 0.0f,
                  glm::sin(m_scale), 0.0f, glm::cos(m_scale), 0.0f,
                  0.0f, 0.0f, 0.0f, 1.0f);

    glm::mat4 fin = m_projection * m_translation * m_rotation;

    this->m_constantBuffer->Write(&fin[0][0]);
    this->m_Renderer->SetConstantBuffer(this->m_constantBuffer);
    this->m_Renderer->Draw(this->m_Model, this->m_pipelineState);
}

void TestLayer::OnAttach() {
}