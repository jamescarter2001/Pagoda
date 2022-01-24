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

        std::string GetName() const override {
            return "WindowResizeEvent";
        }

        virtual std::string ToString() const override {
            std::stringstream output;
            output << this->GetName() << "(" << this->GetX() << " ," << this->GetY() << ")";
            return output.str();
        }

        // EVENT_CLASS_TYPE parameter (WindowResize) is plain text, despite syntax highlighting.
        //EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(eEventCategory::EventCategoryApplication)

    private:
        int m_xValue;
        int m_yValue;
    };

    class PAGODA_API WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() {}

        //EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(eEventCategory::EventCategoryApplication)
    };

    class PAGODA_API CWindowFocusEvent : public Event {
    public:
        CWindowFocusEvent() {}

        //EVENT_CLASS_TYPE(WindowFocus)
        EVENT_CLASS_CATEGORY(eEventCategory::EventCategoryApplication)
    };

    class PAGODA_API CWindowLostFocusEvent : public Event {
    public:
        CWindowLostFocusEvent() {}

        //EVENT_CLASS_TYPE(WindowLostFocus)
        EVENT_CLASS_CATEGORY(eEventCategory::EventCategoryApplication)
    };

    class PAGODA_API CWindowMovedEvent : public Event {
    public:
        CWindowMovedEvent() {}

        //EVENT_CLASS_TYPE(WindowMoved)
        EVENT_CLASS_CATEGORY(eEventCategory::EventCategoryApplication)
    };

    class PAGODA_API CAppTickEvent : public Event {
    public:
        CAppTickEvent() {}

        //EVENT_CLASS_TYPE(AppTick)
        EVENT_CLASS_CATEGORY(eEventCategory::EventCategoryApplication)
    };

    class PAGODA_API CAppRenderEvent : public Event {
    public:
        CAppRenderEvent() {}

        //EVENT_CLASS_TYPE(AppRender)
        EVENT_CLASS_CATEGORY(eEventCategory::EventCategoryApplication)
    };

    class PAGODA_API CAppUpdateEvent : public Event {
    public:
        CAppUpdateEvent() {}

        //EVENT_CLASS_TYPE(AppUpdate)
        EVENT_CLASS_CATEGORY(eEventCategory::EventCategoryApplication)
    };
}