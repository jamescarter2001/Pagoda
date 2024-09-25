#include "pgpch.h"
#include "pg_mirage_manager.h"

#include "mirage/core/renderer/pg_renderer.h"

#include "mirage/platform/d3d11/window/pg_d3d11_window.h"
#include "mirage/platform/d3d12/window/pg_d3d12_window.h"

#include "mirage/platform/d3d/extension/pg_windows_extension.h"

namespace Pagoda::Mirage {
    MirageManager::MirageManager(std::string& appName) : m_appName(appName), m_pWindow(nullptr) {}

    MirageManager::~MirageManager() {}

    void MirageManager::Init() {
        PG_CORE_INFO("[MirageManager] Initializing...");

        // TODO: extract to applicationsetting.lua script.
        switch (Renderer::GetRendererAPI()) {
            case RendererAPI::Direct3D11:
                this->m_pWindow = std::make_shared<D3D11Window>(Mirage::WindowProps(m_appName), [this](HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
                    for (auto e : m_extensions) {
                        if (auto ext = dynamic_cast<WindowsExtension*>(e)) {
                            return ext->WindowProc(hwnd, message, wParam, lParam);
                        }
                    }
                    return 0ll;
                });
                break;
            case RendererAPI::Direct3D12:
                this->m_pWindow = std::make_shared<D3D12Window>(Mirage::WindowProps(m_appName), [this](HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
                    for (auto e : m_extensions) {
                        if (auto ext = dynamic_cast<WindowsExtension*>(e)) {
                            return ext->WindowProc(hwnd, message, wParam, lParam);
                        }
                    }
                    return 0ll;
                });
                break;
        }

        PG_CORE_ASSERT_CRITICAL(this->m_pWindow, "Mirage failed to initialize!");

        PG_CORE_DEBUG("[MirageManager] Graphics API: {}", m_pWindow->GetApiName());

        m_pMirageFactory = m_pWindow->GetMirageFactory();

        PG_CORE_TRACE("[MirageManager] Window successfully initialized");

        m_extensions.push_back(m_pMirageFactory->CreateChiselExtension());

        PG_CORE_TRACE("[MirageManager] Loading extensions...");
        unsigned int i = 0;
        for (auto ex : m_extensions) {
            const unsigned int num = ++i;
            PG_CORE_TRACE("[MirageManager] Loading extension: ({}/{})", num, m_extensions.size());
            ex->Init();
            PG_CORE_TRACE("[MirageManager] Loaded extension: ({}/{})", num, m_extensions.size());
        }

        PG_CORE_TRACE("[MirageManager] Extensions successfully initialized");

        PG_CORE_INFO("[MirageManager] Successfully initialized");
    }

    void MirageManager::ShutDown() {}

    void MirageManager::BeforeUpdate() {
        m_pWindow->BeforeUpdate();

        for (auto ex : m_extensions) {
            ex->BeforeUpdate();
        }
    }

    void MirageManager::OnUpdate() {
        for (auto ex : m_extensions) {
            ex->OnUpdate();
        }
        m_pWindow->OnUpdate();
    }
}