#pragma once
#include "pgCore.h"
#include "Base/Event/pgEvent.h"

namespace Pagoda::Base {
	class PAGODA_API CLayer {
	public:
		CLayer(const std::string& name);
		virtual ~CLayer();
		virtual void OnEvent(CEvent& e) const = 0;
		virtual void OnUpdate() const = 0;
		virtual void OnAttach() const = 0;

		inline std::string GetName() {
			return this->m_Name;
		}
	private:
		std::string m_Name;
	};
}