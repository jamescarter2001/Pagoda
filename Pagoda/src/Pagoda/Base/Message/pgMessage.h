#pragma once
#include "pgpch.h"
#include "pgCore.h"

namespace Pagoda::Base {
	class Message {
	public:
		virtual std::string GetName() const = 0;
		virtual std::string ToString() const {
			return this->GetName();
		}
		bool m_IsHandled = false;
	};
}