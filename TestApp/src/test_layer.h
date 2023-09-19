#pragma once

#include <pagoda.h>

class TestLayer : public  Pagoda::Base::Layer {
	public:
        TestLayer(const std::string& name);
        ~TestLayer();

        virtual void OnEvent(Pagoda::Base::Event& e) const override;
        virtual void OnUpdate() const override;
        virtual void OnAttach() override;

    private:
        Pagoda::Mirage::Model m_Model;
        Pagoda::Mirage::ShaderData m_ShaderData;

        Pagoda::Mirage::Renderer* m_Renderer;
};
