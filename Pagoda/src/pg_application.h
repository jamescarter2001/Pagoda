#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "universe/manager/pg_application_manager.h"

namespace Pagoda {
    class PAGODA_API Application {
    public:
        Application(const std::string& name);
        virtual ~Application();

        void Run();

        inline std::string GetName() {
            return this->m_name;
        }

        inline std::shared_ptr<Universe::ApplicationManager> GetApplicationManager() const {
            return m_applicatonManager;
        }

    private:
        static Application* s_instance;

        std::string m_name;

        std::shared_ptr<Universe::ApplicationManager> m_applicatonManager;
    };
}
