#include "pgpch.h"
#include "pg_application.h"

namespace Pagoda {
    Application* Application::s_instance = nullptr;

    Application::Application(const std::string& name) {
        PG_CORE_ASSERT(!s_instance, "Application already running!");
        this->m_name = name;
        
        m_applicatonManager = std::make_unique<Universe::ApplicationManager>(m_name);
        m_applicatonManager->Start();

        this->s_instance = this;
    }

    Application::~Application() {
        s_instance = nullptr;
    }

    void Application::Run() {
        m_applicatonManager->Run();
    }
}
