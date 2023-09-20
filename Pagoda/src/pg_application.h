#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "base/message/pg_message_dispatcher.h"
#include "base/event/pg_event.h"
#include "base/event/pg_application_event.h"
#include "base/layer/pg_layer.h"
#include "base/layer/pg_layer_stack.h"

#include "mirage/core/window/pg_window.h"
#include "mirage/platform/d3d11/window/pg_d3d11_window.h"

namespace Pagoda {
    class PAGODA_API Application {
    public:
        Application(std::string& name);
        virtual ~Application();

        // Inline functions must be fully declared in header.

        void OnEvent(Base::Event& e);
        bool OnWindowCloseEvent(Base::Event& e);

        void PushLayer(Base::Layer* layer);
        void PushOverlay(Base::Layer* layer);
        void PopLayer(Base::Layer* layer);
        void PopOverlay(Base::Layer* layer);

        bool Setup();
        void Run();

        inline std::string GetName() {
            return this->m_Name;
        }

    private:
        static Application* s_Instance;

        std::string m_Name;
        bool m_IsRunning;
        Base::LayerStack m_LayerStack;

        Mirage::Window* m_Window;
    };
}