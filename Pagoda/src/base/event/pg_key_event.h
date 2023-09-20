#pragma once
#include "pg_event.h"

namespace Pagoda::Base {
    class PAGODA_API CKeyEvent : public Event {
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
        CKeyEvent(int keyCode) {
            this->m_keyCode = keyCode;
        }
        int m_keyCode;
    };

    class CKeyPressedEvent : public CKeyEvent {
    public:
        CKeyPressedEvent(int keyCode, int repeatCount) : CKeyEvent(keyCode) {
            this->m_keyRepeatCount = repeatCount;
        }
        EVENT_CLASS_TYPE(KeyPressed)
    private:
        int m_keyRepeatCount;
    };

    class CKeyReleasedEvent : public CKeyEvent {
    public:
        CKeyReleasedEvent(int keyCode) : CKeyEvent(keyCode) {}
        EVENT_CLASS_TYPE(KeyReleased)
    };
}