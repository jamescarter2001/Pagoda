#pragma once
#include "pgEvent.h"

namespace Pagoda::Base {
	class CMouseButtonEvent : public Event {
	public:
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton)
			inline int GetMouseButton() const {
			return m_Button;
		}
	protected:
		CMouseButtonEvent(int button) {
			m_Button = button;
		}
		int m_Button;
	};

	class CMouseButtonPressedEvent : public CMouseButtonEvent {
	public:
		CMouseButtonPressedEvent(int button) : CMouseButtonEvent(button) {}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class CMouseButtonReleasedEvent : public CMouseButtonEvent {
	public:
		CMouseButtonReleasedEvent(int button) : CMouseButtonEvent(button) {}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

	class CMouseMovedEvent : public Event {
	public:
		CMouseMovedEvent(float x, float y) {
			this->m_MouseX = x;
			this->m_MouseY = y;
		}

		EVENT_CLASS_TYPE(MouseMoved)
			EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
			float GetMouseX() const {
			return m_MouseX;
		}

		float GetMouseY() const {
			return m_MouseY;
		}

		virtual std::string ToString() const override {
			std::stringstream output;
			output << this->GetName() << "(" << this->GetMouseX() << ", " << this->GetMouseY() << ")";
			return output.str();
		}

	private:
		float m_MouseX;
		float m_MouseY;
	};

	class CMouseScrollEvent : public Event {
	public:
		CMouseScrollEvent(float x, float y) {
			this->m_MouseX = x;
			this->m_MouseY = y;
		}

		EVENT_CLASS_TYPE(MouseMoved)
			EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

			inline float GetMouseX() const {
			return m_MouseX;
		}

		inline float GetMouseY() const {
			return m_MouseY;
		}
	private:
		float m_MouseX;
		float m_MouseY;
	};
}