#pragma once
#include "pgEvent.h"

namespace Pagoda::Base {
	class PAGODA_API WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(int x, int y) {
			this->m_xValue = x;
			this->m_yValue = y;
		}

		int GetX() const {
			return m_xValue;
		}

		int GetY() const {
			return m_yValue;
		}

		virtual std::string ToString() const override {
			std::stringstream output;
			output << this->GetName() << "(" << this->GetX() << " ," << this->GetY() << ")";
			return output.str();
		}

		// EVENT_CLASS_TYPE parameter (WindowResize) is plain text, despite syntax highlighting.
		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(eEventCategory::EventCategoryApplication)

	private:
		int m_xValue;
		int m_yValue;
	};

	class PAGODA_API WindowCloseEvent : public Event {
	public:

		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(eEventCategory::EventCategoryApplication)
	};

	class PAGODA_API WindowFocusEvent : public Event {
	public:

		WindowFocusEvent() {}

		EVENT_CLASS_TYPE(WindowFocus)
		EVENT_CLASS_CATEGORY(eEventCategory::EventCategoryApplication)
	};

	class PAGODA_API WindowLostFocusEvent : public Event {
	public:

		WindowLostFocusEvent() {}

		EVENT_CLASS_TYPE(WindowLostFocus)
		EVENT_CLASS_CATEGORY(eEventCategory::EventCategoryApplication)
	};

	class PAGODA_API WindowMovedEvent : public Event {
	public:

		WindowMovedEvent() {}

		EVENT_CLASS_TYPE(WindowMoved)
		EVENT_CLASS_CATEGORY(eEventCategory::EventCategoryApplication)
	};

	class PAGODA_API AppTickEvent : public Event {
	public:

		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(eEventCategory::EventCategoryApplication)
	};

	class PAGODA_API AppRenderEvent : public Event {
	public:

		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(eEventCategory::EventCategoryApplication)
	};

	class PAGODA_API AppUpdateEvent : public Event {
	public:

		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(eEventCategory::EventCategoryApplication)
	};
}