#include "pgpch.h"
#include "pgApplication.h"

namespace Pagoda {

	Application* Application::s_Instance = nullptr;

	Application::Application(std::string& name) {
		PG_CORE_ASSERT(!s_Instance, "Application already running!");
		this->m_Name = name;
		this->m_IsRunning = false;
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
		Base::EventDispatcher dispatcher(e);

		dispatcher.Dispatch<Base::WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowCloseEvent));
	}

	void Application::Setup() {
		PG_CORE_TRACE("Starting engine application: {}", m_Name);
	}

	void Application::Run() {
		this->Setup();
		PG_CORE_INFO("System initialization success.");
		this->m_IsRunning = true;
		while (m_IsRunning) {
		}
	}
}