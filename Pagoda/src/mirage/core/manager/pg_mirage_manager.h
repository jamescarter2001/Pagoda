#pragma once
#include "pgpch.h"

#include "base/manager/pg_abstract_manager.h"
#include "mirage/core/window/pg_window.h"

namespace Pagoda::Mirage {
    class PAGODA_API MirageManager : public Base::AbstractManager {
    public:
        MirageManager(std::string& appName);
        virtual ~MirageManager();
        virtual void Init() override;
        virtual void ShutDown() override;

        void BeforeUpdate() override;
        void OnUpdate() override;

        inline std::shared_ptr<Window> GetWindow() {
            return m_pWindow;
        }

        inline std::shared_ptr<MirageFactory> GetMirageFactory() const {
            return m_pMirageFactory;
        }

    private:
        std::string& m_appName;

        std::shared_ptr<Window> m_pWindow;
        std::shared_ptr<MirageFactory> m_pMirageFactory;

        std::vector<Extension*> m_extensions;
    };
}
