#pragma once
#include "pgCore.h"
#include "Base/Log/pgLog.h"

namespace Pagoda {
	class PAGODA_API pgApplication {
	public:
		pgApplication(std::string& name);
		virtual ~pgApplication();

		// Inline functions must be fully declared in header.
		inline std::string GetName() {
			return this->m_Name;
		}

		void Setup();
		void Run();

	private:
		static pgApplication* s_Instance;

		std::string m_Name;
		bool m_IsRunning;
	};
}