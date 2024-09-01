#include "pgpch.h"
#include "pg_application_manager.h"

#include "pg_core.h"

#include "base/message/pg_message_dispatcher.h"

namespace Pagoda::Universe {
    ApplicationManager::ApplicationManager(std::string& appName) : m_appName(appName) {}
    ApplicationManager::~ApplicationManager() {}

    void ApplicationManager::Init() {
        PG_CORE_INFO("Starting engine application: {}", m_appName);

        // --- Mirage ---
        PG_CORE_TRACE("Initialising Mirage subsystem...");
        m_pMirageManager = std::make_shared<Mirage::MirageManager>(m_appName);
        m_pMirageManager->Init();
        m_pWindow = m_pMirageManager->GetWindow();

        if (auto w = m_pWindow.lock()) {
            w->SetEventCallback([this](Base::Event& e) {
                this->OnEvent(e);
            });
        }

        PG_CORE_INFO("Mirage initialisation successful");
        // --- Mirage ---

        m_pAppCtx = ApplicationContext(m_pMirageManager);
        m_layerStack = std::make_unique<LayerStack>(m_pAppCtx);

        PG_CORE_INFO("Started engine application: {}", m_appName);
    }

    bool ApplicationManager::OnWindowCloseEvent(Base::Event& e) {
        ShutDown();
        return true;
    }

    void ApplicationManager::OnEvent(Base::Event& e) {
        PG_CORE_DEBUG("Event {}", e.ToString());
        const Base::MessageDispatcher dispatcher(e);

        // System
        dispatcher.Dispatch<Base::WindowCloseEvent>(BIND_EVENT_FN(ApplicationManager::OnWindowCloseEvent));

        // Layer
        for (std::vector<Layer*>::iterator it = this->m_layerStack->end(); it != this->m_layerStack->begin();) {
            (*--it)->OnEvent(m_pAppCtx, e);
        }
    }

    void ApplicationManager::PushLayer(Layer* layer) {
        this->m_layerStack->PushLayer(layer);
    }

    void ApplicationManager::PushOverlay(Layer* layer) {
        this->m_layerStack->PushOverlay(layer);
    }

    void ApplicationManager::PopLayer(Layer* layer) {
        this->m_layerStack->PopLayer(layer);
    }

    void ApplicationManager::PopOverlay(Layer* layer) {
        this->m_layerStack->PopOverlay(layer);
    }

    void ApplicationManager::Run() {
        m_isRunning = true;

        while (m_isRunning) {
            if (auto w = m_pWindow.lock()) {
                w->BeforeUpdate();
                for (Layer* layer : *m_layerStack) {
                    layer->OnUpdate(m_pAppCtx);
                }
                w->OnUpdate();
            }
        }
        PG_CORE_INFO("Shutting down...");
    }

    void ApplicationManager::ShutDown() {
        m_isRunning = false;
    }
}