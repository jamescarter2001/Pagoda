#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "base/message/pg_message_dispatcher.h"
#include "base/event/pg_event.h"
#include "base/event/pg_application_event.h"
#include "base/layer/pg_layer.h"
#include "base/layer/pg_layer_stack.h"

#include "mirage/core/window/pg_window.h"
#include "mirage/core/renderer/pg_renderer.h"

#include "mirage/platform/d3d11/window/pg_d3d11_window.h"
#include "mirage/platform/d3d12/window/pg_d3d12_window.h"

namespace Pagoda {
    class PAGODA_API Application {
    public:
        Application(const std::string& name);
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
            return this->m_name;
        }

    private:
        static Application* s_instance;

        std::string m_name;
        bool m_isRunning;
        Base::LayerStack m_layerStack;

        Mirage::Window* m_window;
    };
}
