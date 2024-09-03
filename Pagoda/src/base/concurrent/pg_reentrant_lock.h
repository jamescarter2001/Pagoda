#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "pg_concurrent.h"
#include "pg_lock.h"

namespace Pagoda::Base {
    class ReentrantLock : public Lock {
    public:

        ReentrantLock() : m_threadId(0), m_refCount(0) {}

        std::size_t getThreadId() {
            std::hash<std::thread::id> hasher;
            return hasher(std::this_thread::get_id());
        }

        virtual inline void Acquire() override {
            std::size_t tid = getThreadId();

            if (m_threadId != tid) {
                std::size_t unlockValue = 0;
                while (!m_threadId.compare_exchange_weak(unlockValue, tid, std::memory_order_acquire, std::memory_order_relaxed)) {
                    unlockValue = 0;
                    THREAD_PAUSE();
                }
                PG_CORE_ASSERT(m_refCount == 0, "Reentrant Lock RefCount is not 0");
            }

            ++m_refCount;
        }

        virtual inline bool TryAcquire() override {
            std::size_t tid = getThreadId();
            bool success = false;

            if (this->m_threadId == tid) {
                success = true;
            } else {
                std::size_t unlockValue = 0;
                success = m_threadId.compare_exchange_strong(unlockValue, tid, std::memory_order_acquire, std::memory_order_relaxed);
            }

            if (success) {
                ++m_refCount;
            }

            return success;
        }

        virtual inline void Release() override {
            std::size_t tid = getThreadId();
            PG_CORE_ASSERT(tid == m_threadId.load(std::memory_order_relaxed), "Thread attempting to release a lock it does not hold");

            --m_refCount;

            if (m_refCount == 0) {
                m_threadId.store(0, std::memory_order_release);
            }
        }

    private:
        std::atomic<size_t> m_threadId;
        std::int32_t m_refCount;
    };
}