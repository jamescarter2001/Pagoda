#pragma once
#include "pgpch.h"
#include "pgCore.h"
#include "Base/Log/pgLog.h"
#include "Base/Event/pgEvent.h"
#include "Base/Event/pgApplicationEvent.h"
#include "Base/Event/pgEventDispatcher.h"
#include "Base/Layer/pgLayerStack.h"

namespace Pagoda {
	class PAGODA_API Application {
	public:
		Application(std::string& name);
		virtual ~Application();

		// Inline functions must be fully declared in header.
		inline std::string GetName() {
			return this->m_Name;
		}

		void OnEvent(Base::Event& e);
		bool OnWindowCloseEvent(Base::Event& e);

		bool TESTMETHOD(Base::Event& e) {
			//PG_CORE_WARNING("AAAAA");
			return true;
		}

		bool Setup();
		void Run();

	private:
		static Application* s_Instance;

		std::string m_Name;
		bool m_IsRunning;
		Base::LayerStack m_LayerStack;
	};
}