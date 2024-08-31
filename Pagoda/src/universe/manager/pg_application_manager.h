#pragma once
#include "pgpch.h"

#include "universe/layer/pg_layer.h"
#include "universe/layer/pg_layer_stack.h"

#include "mirage/core/window/pg_window.h"
#include "mirage/pg_mirage_factory.h"
#include "mirage/core/manager/pg_mirage_manager.h"

#include "universe/context/pg_application_context.h"

namespace Pagoda::Universe {
    class PAGODA_API ApplicationManager {
    public:
        ApplicationManager(std::string& appName);
        ~ApplicationManager();

        bool Start();
        void Run();
        bool ShutDown();

        void OnEvent(Base::Event& e);
        bool OnWindowCloseEvent(Base::Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* layer);

    private:
        // Managers
        std::shared_ptr<Mirage::MirageManager> m_pMirageManager;

        std::weak_ptr<Mirage::Window> m_pWindow;

        ApplicationContext m_pAppCtx;

        std::unique_ptr<LayerStack> m_layerStack;

        std::string& m_appName;
        bool m_isRunning = false;
    };
}
