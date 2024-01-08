#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "base/event/pg_event.h"
#include "base/event/pg_application_event.h"

#include "mirage/core/window/pg_window_data.h"

namespace Pagoda::Mirage {
    class PAGODA_API Window {
    public:
        Window(const WindowProps& props) {
            m_windowData.Title = props.Title;
            m_windowData.Width = props.Width;
            m_windowData.Height = props.Height;
        }

        virtual ~Window() {
        }

        virtual void Init() = 0;
        virtual void BeforeUpdate() = 0;
        virtual void OnUpdate() = 0;

        inline int GetWidth() const {
            return m_windowData.Width;
        }

        inline int GetHeight() const {
            return m_windowData.Height;
        }

        inline std::string GetTitle() {
            return m_windowData.Title;
        }

        inline WindowData* GetWindowData() {
            return &m_windowData;
        }

        void SetEventCallback(const std::function<void(Base::Event&)>& callback) {
            this->m_windowData.EventCallback = callback;
        }

    protected:
        WindowData m_windowData;
    };
}
