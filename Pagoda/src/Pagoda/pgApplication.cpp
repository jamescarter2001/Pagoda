#include "pgpch.h"
#include "pgApplication.h"

namespace Pagoda {

	pgApplication* pgApplication::s_Instance = nullptr;

	pgApplication::pgApplication(std::string& name) {
		PG_CORE_ASSERT(!s_Instance, "Application already running!");
		this->m_Name = name;
		this->m_IsRunning = false;
		this->s_Instance = this;
	}

	pgApplication::~pgApplication() {
		s_Instance = nullptr;
	}

	void pgApplication::Setup() {
		PG_CORE_TRACE("Starting engine application: {}", m_Name);
	}

	void pgApplication::Run() {
		this->Setup();
		PG_CORE_INFO("System initialization success.");
		this->m_IsRunning = true;
		while (m_IsRunning) {

		}
	}

}