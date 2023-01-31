#pragma once
#include "pgpch.h"
#include "pgCore.h"
#include "Base/Log/pgLog.h"
#include "Base/Event/pgEvent.h"
#include "Base/Event/pgApplicationEvent.h"
#include "Base/Message/pgMessageDispatcher.h"
#include "Base/Layer/pgLayerStack.h"

#include "Luanne/pgLuanne.h"

#include "Mirage/pgMirageFactory.h"

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
        Luanne::LuaHandler* m_LuaHandler;
    };
}