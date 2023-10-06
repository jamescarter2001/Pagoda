#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "base/event/pg_event.h"
#include "base/event/pg_application_event.h"

#include "mirage/core/window/pg_window_data.h"
#include "mirage/pg_mirage_factory.h"

namespace Pagoda::Mirage {
    class PAGODA_API Window {
    public:
        Window(const WindowProps& props) {
            m_WindowData.Title = props.Title;
            m_WindowData.Width = props.Width;
            m_WindowData.Height = props.Height;
        }

        virtual ~Window() {}

        virtual MirageFactory* Init() = 0;
        virtual void BeforeUpdate() = 0;
        virtual void OnUpdate() = 0;

        inline int GetWidth() {
            return m_WindowData.Width;
        }

        inline int GetHeight() {
            return m_WindowData.Height;
        }

        inline std::string GetTitle() {
            return m_WindowData.Title;
        }

        inline WindowData* GetWindowData() {
            return &m_WindowData;
        }

        void SetEventCallback(const std::function<void(Base::Event& m)>& callback) {
            this->m_WindowData.EventCallback = callback;
        }

    protected:
        WindowData m_WindowData;
    };
}