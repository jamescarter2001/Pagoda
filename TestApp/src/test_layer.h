#pragma once

#include <pagoda.h>

class TestLayer : public  Pagoda::Base::Layer {
	public:
        TestLayer(const std::string& name, Pagoda::Mirage::MirageFactory* mf);
        ~TestLayer();

        virtual void OnEvent(Pagoda::Base::Event& e) const override;
        virtual void OnUpdate() override;
        virtual void OnAttach() override;

    private:
        Pagoda::Mirage::MirageFactory* m_mirageFactory;

        Pagoda::Mirage::Model m_Model;
        Pagoda::Mirage::PipelineState* m_pipelineState;

        Pagoda::Mirage::ConstantBuffer<float>* m_constantBuffer;

        Pagoda::Mirage::Renderer* m_Renderer;

        glm::mat4 m_translation;
        glm::mat4 m_projection;
        glm::mat4 m_identity;

        float m_scale = 0.0f;
};
