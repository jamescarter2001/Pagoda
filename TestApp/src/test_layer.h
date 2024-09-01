#pragma once

#include <pagoda.h>

class TestLayer : public  Pagoda::Universe::Layer {
	public:
        TestLayer(const std::string& name);
        ~TestLayer();

        virtual void OnEvent(Pagoda::Universe::ApplicationContext& ctx, Pagoda::Base::Event& e) const override;
        virtual void OnUpdate(Pagoda::Universe::ApplicationContext& ctx) override;
        virtual void OnAttach(Pagoda::Universe::ApplicationContext& ctx) override;

    private:
        Pagoda::Mirage::Model m_Model;
        Pagoda::Mirage::PipelineState* m_pipelineState;

        Pagoda::Mirage::ConstantBuffer<float>* m_constantBuffer;

        Pagoda::Mirage::Renderer* m_Renderer;

        glm::mat4 m_translation;
        glm::mat4 m_identity;

        float m_scale = 0.0f;
};
