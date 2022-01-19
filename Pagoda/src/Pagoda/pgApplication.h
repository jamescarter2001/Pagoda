#pragma once
#include "pgCore.h"
#include "Base/Log/pgLog.h"
#include "Base/Event/pgApplicationEvent.h"

namespace Pagoda {
	class PAGODA_API Application {
	public:
		Application(std::string& name);
		virtual ~Application();

		// Inline functions must be fully declared in header.
		inline std::string GetName() {
			return this->m_Name;
		}

		void Setup();
		void Run();

	private:
		static Application* s_Instance;

		std::string m_Name;
		bool m_IsRunning;
	};
}