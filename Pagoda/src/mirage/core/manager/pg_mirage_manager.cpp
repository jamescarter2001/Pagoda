#include "pgpch.h"
#include "pg_mirage_manager.h"

#include "mirage/core/renderer/pg_renderer.h"

#include "mirage/platform/d3d11/window/pg_d3d11_window.h"
#include "mirage/platform/d3d12/window/pg_d3d12_window.h"

namespace Pagoda::Mirage {
    MirageManager::MirageManager(std::string& appName) : m_appName(appName) {}

    MirageManager::~MirageManager() {}

    void MirageManager::Init() {
        PG_CORE_TRACE("[MirageManager] Initializing...");
        // TODO: extract to applicationsetting.lua script.
        switch (Mirage::Renderer::GetRendererAPI()) {
            case Mirage::RendererAPI::Direct3D11:
                this->m_pWindow = std::make_shared<D3D11Window>(Mirage::WindowProps(m_appName));
                break;
            case Mirage::RendererAPI::Direct3D12:
                this->m_pWindow = std::make_shared<D3D12Window>(Mirage::WindowProps(m_appName));
                break;
        }
        PG_CORE_ASSERT_CRITICAL(this->m_pWindow, "Mirage failed to initialize!");

        m_pMirageFactory = m_pWindow->GetMirageFactory();

        PG_CORE_INFO("[MirageManager] Successfully initialized");
    }

    void MirageManager::ShutDown() {}

    void MirageManager::BeforeUpdate() {
        m_pWindow.get()->BeforeUpdate();
    }

    void MirageManager::OnUpdate() {
        m_pWindow.get()->OnUpdate();
    }
}