#include "pgpch.h"
#include "pgApplication.h"

namespace Pagoda {

    Application* Application::s_Instance = nullptr;

    Application::Application(std::string& name) {
        PG_CORE_ASSERT(!s_Instance, "Application already running!");
        this->m_Name = name;
        this->m_IsRunning = false;

        this->m_Window = Mirage::Window::Create();
        this->m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

        this->s_Instance = this;
    }

    Application::~Application() {
        s_Instance = nullptr;
    }

    bool Application::OnWindowCloseEvent(Base::Event& e) {
        this->m_IsRunning = false;
        return true;
    }

    void Application::OnEvent(Base::Event& e) {
        Base::MessageDispatcher dispatcher(e);

        // System
        dispatcher.Dispatch<Base::WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowCloseEvent));

        // Layer
        for (std::vector<Base::Layer*>::iterator it = this->m_LayerStack.end(); it != this->m_LayerStack.begin();) {
            (*--it)->OnEvent(e);
        }
    }

    void Application::PushLayer(Base::Layer* layer) {
        this->m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Base::Layer* layer) {
        this->m_LayerStack.PushOverlay(layer);
    }

    void Application::PopLayer(Base::Layer* layer) {
        this->m_LayerStack.PopLayer(layer);
    }

    void Application::PopOverlay(Base::Layer* layer) {
        this->m_LayerStack.PopOverlay(layer);
    }

    bool Application::Setup() {
        PG_CORE_TRACE("Starting engine application: {}", m_Name);
        return true;
    }

    void Application::Run() {
        PG_CORE_ASSERT_CRITICAL(this->Setup(), "System failed to initialize.");
        PG_CORE_INFO("System initialization success.");
        this->m_IsRunning = true;

        while (m_IsRunning) {
            for (Base::Layer* layer : this->m_LayerStack) {
                layer->OnUpdate();
            }
            this->m_Window->OnUpdate();
        }
        PG_CORE_INFO("Shutting down...");
    }
}