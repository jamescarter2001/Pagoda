#pragma once
#include "pgpch.h"
#include "Base/Event/pgEvent.h"

namespace Pagoda::Mirage {
    struct WindowProps {
        std::string Title = "Pagoda";
        int Width = 1280;
        int Height = 720;

        WindowProps() {}

        WindowProps(std::string Title) {
            this->Title = Title;
        }

        WindowProps(std::string Title, int Width, int Height) {
            this->Title = Title;
            this->Width = Width;
            this->Height = Height;
        }
    };

    struct WindowData {
        std::string Title;
        int Width, Height;
        bool VSync;
        std::function<void(Base::Event& m)> EventCallback = [](Base::Event& m) {
            PG_CORE_WARNING("Application event callback not set!");
        };
    };

    class Window {
    public:
        Window(const WindowProps& props) {
            m_WindowData.Title = props.Title;
            m_WindowData.Width = props.Width;
            m_WindowData.Height = props.Height;
        }

        virtual ~Window() {}

        virtual void Init() = 0;
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
       
        void SetEventCallback(const std::function<void(Base::Event& m)>& callback) {
            this->m_WindowData.EventCallback = callback;
        }

        static Window* Create(const WindowProps& props = WindowProps());

    protected:
        WindowData m_WindowData;
    };
}