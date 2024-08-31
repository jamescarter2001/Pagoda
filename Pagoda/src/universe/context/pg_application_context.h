#pragma once
#include "pgpch.h"

#include "mirage/core/manager/pg_mirage_manager.h"

namespace Pagoda::Universe {
    struct ApplicationContext {
    public:
        ApplicationContext() {}
        ApplicationContext(std::shared_ptr<Mirage::MirageManager> mirageManager) : m_pMirageManager(mirageManager) {}
        ~ApplicationContext() {}

        inline std::shared_ptr<Mirage::MirageManager> GetMirageManager() const {
            return m_pMirageManager;
        }

    private:
        std::shared_ptr<Mirage::MirageManager> m_pMirageManager;
    };
}
