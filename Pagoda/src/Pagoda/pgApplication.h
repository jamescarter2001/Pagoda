#pragma once
#include "pgpch.h"
#include "pgCore.h"
#include "Base/Log/pgLog.h"
#include "Base/Event/pgEvent.h"
#include "Base/Event/pgApplicationEvent.h"
#include "Base/Event/pgEventDispatcher.h"
#include "Base/Layer/pgLayerStack.h"

namespace Pagoda {
	class PAGODA_API CApplication {
	public:
		CApplication(std::string& name);
		virtual ~CApplication();

		// Inline functions must be fully declared in header.
		inline std::string GetName() {
			return this->m_Name;
		}

		void OnEvent(Base::CEvent& e);
		bool OnWindowCloseEvent(Base::CEvent& e);

		bool TESTMETHOD(Base::CEvent& e) {
			//PG_CORE_WARNING("AAAAA");
			return true;
		}

		bool Setup();
		void Run();

	private:
		static CApplication* s_Instance;

		std::string m_Name;
		bool m_IsRunning;
		Base::CLayerStack m_LayerStack;
	};
}