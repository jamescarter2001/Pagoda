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
            m_windowData.Title = props.Title;
            m_windowData.Width = props.Width;
            m_windowData.Height = props.Height;
        }

        virtual ~Window() {
        }

        virtual std::shared_ptr<MirageFactory> Init() = 0;
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

        inline std::shared_ptr<MirageFactory> GetMirageFactory() const {
            return m_mirageFactory;
        }

        void SetEventCallback(const std::function<void(Base::Event&)>& callback) {
            this->m_windowData.EventCallback = callback;
        }

    protected:
        WindowData m_windowData;
        std::shared_ptr<MirageFactory> m_mirageFactory;
    };
}
