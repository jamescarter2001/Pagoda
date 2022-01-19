#pragma once
#include "pgEvent.h"

namespace Pagoda::Base {
	class MouseButtonEvent : public Event {
	public:
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton)
			inline int GetMouseButton() const {
			return m_Button;
		}
	protected:
		MouseButtonEvent(int button) {
			m_Button = button;
		}
		int m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float x, float y) {
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

	class MouseScrollEvent : public Event {
	public:
		MouseScrollEvent(float x, float y) {
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