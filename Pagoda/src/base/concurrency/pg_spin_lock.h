#pragma once
#include "pgpch.h"

#include "pg_concurrency.h"

namespace Pagoda::Base {
    class SpinLock {
    public:
        SpinLock() {
        }

        virtual ~SpinLock() {
        }

        inline bool try_acquire() {
            const bool alreadyLocked = this->m_lock.test_and_set(std::memory_order_acquire);

            return !alreadyLocked;
        }

        inline void acquire() {
            while (!this->try_acquire()) {
                THREAD_PAUSE();
            }
        }

        inline void release() {
            this->m_lock.clear(std::memory_order_release);
        }

    private:
        std::atomic_flag m_lock;
    };
}
