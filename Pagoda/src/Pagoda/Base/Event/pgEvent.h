#pragma once
#include "pgpch.h"
#include "pgCore.h"

namespace Pagoda::Base {

	// Auto-generate
#define EVENT_CLASS_TYPE(type) static eEventType GetStaticEventType() { return eEventType::##type; }\
									   virtual eEventType GetEventType() const override { return GetStaticEventType(); }\
										virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(bitfield) virtual int GetCategories() const override { return (int) bitfield; }

	enum class eEventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum eEventCategory {
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

	class Event {
	public:
		virtual eEventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategories() const = 0;
		virtual std::string ToString() const {
			return this->GetName();
		}
	private:
		bool m_IsHandled = false;
	};
}