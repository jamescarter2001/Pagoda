#pragma once
#include "pgEvent.h"

namespace Pagoda::Base {
	class PAGODA_API KeyEvent : public Event {
	public:
		inline int getKeyCode() const {
			return m_keyCode;
		}

		virtual std::string ToString() const override {
			std::stringstream output;
			output << this->GetName() << "(" << this->getKeyCode() << ")";
			return output.str();
		}

		EVENT_CLASS_CATEGORY(eEventCategory::EventCategoryInput | eEventCategory::EventCategoryKeyboard)

			inline int GetKeyCode() const {
			return m_keyCode;
		}
	protected:
		KeyEvent(int keyCode) {
			this->m_keyCode = keyCode;
		}
		int m_keyCode;
	};

	class KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(int keyCode, int repeatCount) : KeyEvent(keyCode) {
			this->m_keyRepeatCount = repeatCount;
		}
		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_keyRepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {}
		EVENT_CLASS_TYPE(KeyReleased)
	};
}