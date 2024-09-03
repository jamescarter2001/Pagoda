#pragma once
#include "pgpch.h"

#include "pg_concurrent.h"
#include "pg_lock.h"

namespace Pagoda::Base {
    class SpinLock : public Lock {
    public:
        SpinLock() {
        }

        virtual ~SpinLock() {
        }

        virtual inline bool TryAcquire() override {
            const bool alreadyLocked = this->m_lock.test_and_set(std::memory_order_acquire);

            return !alreadyLocked;
        }

        virtual inline void Acquire() override {
            while (!this->TryAcquire()) {
                THREAD_PAUSE();
            }
        }

        virtual inline void Release() override {
            this->m_lock.clear(std::memory_order_release);
        }

    private:
        std::atomic_flag m_lock;
    };
}
