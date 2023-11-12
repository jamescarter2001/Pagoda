#pragma once
#include "pgpch.h"

#include "pg_spin_lock.h"

namespace Pagoda::Base {
    class ScopedLock {
    public:
        ScopedLock(SpinLock& l)
            : m_pLock(&l) {
            this->m_pLock->Acquire();
        }

        virtual ~ScopedLock() {
            this->m_pLock->Release();
        }

    private:
        SpinLock* m_pLock;
    };
}
