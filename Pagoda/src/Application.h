#pragma once
#include "Core.h"

namespace Pagoda {
	class Application {
		Application();
		virtual ~Application();

		void Setup();
		void Run();

	private:
		static Application* s_Instance;

		bool m_IsRunning;
	};
}