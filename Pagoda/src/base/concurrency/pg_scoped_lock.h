#pragma once
#include "pgpch.h"

#include "pg_spin_lock.h"

namespace Pagoda::Base {
    class ScopedLock {
    public:
        explicit ScopedLock(SpinLock& l)
            : m_p_lock(&l) {
            this->m_p_lock->acquire();
        }

        virtual ~ScopedLock() {
            this->m_p_lock->release();
        }

    private:
        SpinLock* m_p_lock;
    };
}
