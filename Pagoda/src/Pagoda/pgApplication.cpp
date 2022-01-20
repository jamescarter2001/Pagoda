#include "pgpch.h"
#include "pgApplication.h"

namespace Pagoda {

	CApplication* CApplication::s_Instance = nullptr;

	CApplication::CApplication(std::string& name) {
		PG_CORE_ASSERT(!s_Instance, "Application already running!");
		this->m_Name = name;
		this->m_IsRunning = false;
		this->s_Instance = this;
	}

	CApplication::~CApplication() {
		s_Instance = nullptr;
	}

	bool CApplication::OnWindowCloseEvent(Base::CEvent& e) {
		this->m_IsRunning = false;
		return true;
	}

	void CApplication::OnEvent(Base::CEvent& e) {
		Base::CEventDispatcher dispatcher(e);

		// System
		dispatcher.Dispatch<Base::CWindowCloseEvent>(BIND_EVENT_FN(CApplication::OnWindowCloseEvent));

		// Layer
		for (std::vector<Base::CLayer*>::iterator it = this->m_LayerStack.end(); it != this->m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
		}
	}

	bool CApplication::Setup() {
		PG_CORE_TRACE("Starting engine application: {}", m_Name);
		return true;
	}

	void CApplication::Run() {
		PG_CORE_ASSERT_CRITICAL(this->Setup(), "System failed to initialize.");
		PG_CORE_INFO("System initialization success.");
		this->m_IsRunning = true;
		while (m_IsRunning) {
			for (Base::CLayer* layer : this->m_LayerStack) {
				layer->OnUpdate();
			}
		}
		PG_CORE_INFO("Shutting down...");
	}
}