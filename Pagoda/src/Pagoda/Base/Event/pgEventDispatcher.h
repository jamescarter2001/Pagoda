#pragma once
#include "pgpch.h"
#include "pgCore.h"
#include "Base/Log/pgLog.h"

#include "pgEvent.h"

namespace Pagoda::Base {
	class PAGODA_API CEventDispatcher {
	public:
		CEventDispatcher(CEvent& e) : m_Event(e) {}

		// methods using templates must be fully declared in header to avoid linker error.
		template<typename T>
		void Dispatch(std::function<bool(T&)> func) const {
			if (this->m_Event.GetEventType() == T::GetStaticEventType()) {
				//PG_CORE_TRACE("Processing event: ", this->m_Event.GetName());
				this->m_Event.m_IsHandled = func((T&)this->m_Event);
			}
		}

	private:
		CEvent &m_Event;
	};
}