#pragma once
#include "pgpch.h"

#include "pg_lock.h"

namespace Pagoda::Base {
    class ScopedLock {
    public:
        ScopedLock(Lock& l)
            : m_pLock(&l) {
            this->m_pLock->Acquire();
        }

        virtual ~ScopedLock() {
            this->m_pLock->Release();
        }

    private:
        Lock* m_pLock;
    };
}
