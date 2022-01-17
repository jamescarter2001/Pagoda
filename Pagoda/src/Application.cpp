#include "Application.h"

namespace Pagoda {

	Application::Application() {
		PG_CORE_ASSERT(!s_Instance, "Application already running!");
		s_Instance = this;
	}

	Application::~Application() {

	}

	void Application::Setup() {

	}

	void Application::Run() {

	}

}