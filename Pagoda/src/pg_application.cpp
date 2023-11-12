#include "pgpch.h"
#include "pg_application.h"

namespace Pagoda {
    Application* Application::s_instance = nullptr;

    Application::Application(const std::string& name) {
        PG_CORE_ASSERT(!s_instance, "Application already running!");
        this->m_name = name;
        this->m_isRunning = false;

        // Mirage
        PG_CORE_TRACE("Initialising Mirage subsystem...");
        this->m_window = new Mirage::D3D12Window(Mirage::WindowProps(name));
        this->m_window->SetEventCallback([this](Base::Event& e) {
            this->OnEvent(e);
        });
        this->m_mirageFactory = this->m_window->GetMirageFactory();
        PG_CORE_INFO("Mirage initialisation successful");

        this->s_instance = this;
    }

    Application::~Application() {
        s_instance = nullptr;
    }

    bool Application::OnWindowCloseEvent(Base::Event& e) {
        this->m_isRunning = false;
        return true;
    }

    void Application::OnEvent(Base::Event& e) {
        PG_CORE_DEBUG("Event {}", e.ToString());
        const Base::MessageDispatcher dispatcher(e);

        // System
        dispatcher.Dispatch<Base::WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowCloseEvent));

        // Layer
        for (std::vector<Base::Layer*>::iterator it = this->m_layerStack.end(); it != this->m_layerStack.begin();) {
            (*--it)->OnEvent(e);
        }
    }

    void Application::PushLayer(Base::Layer* layer) {
        this->m_layerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Base::Layer* layer) {
        this->m_layerStack.PushOverlay(layer);
    }

    void Application::PopLayer(Base::Layer* layer) {
        this->m_layerStack.PopLayer(layer);
    }

    void Application::PopOverlay(Base::Layer* layer) {
        this->m_layerStack.PopOverlay(layer);
    }

    bool Application::Setup() {
        PG_CORE_TRACE("Starting engine application: {}", this->m_name);
        return true;
    }

    void Application::Run() {
        PG_CORE_ASSERT_CRITICAL(this->Setup(), "System failed to initialise.");
        PG_CORE_INFO("System initialisation success");
        this->m_isRunning = true;

        while (m_isRunning) {
            this->m_window->BeforeUpdate();
            for (Base::Layer* layer : this->m_layerStack) {
                layer->OnUpdate();
            }
            this->m_window->OnUpdate();
        }
        PG_CORE_INFO("Shutting down...");
    }
}
