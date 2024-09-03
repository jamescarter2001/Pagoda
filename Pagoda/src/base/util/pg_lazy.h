#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

namespace Pagoda::Base {

    template<typename T>
    class Lazy {
    public:
        Lazy() {}
        ~Lazy() {
            if (m_underlying != nullptr) delete m_underlying;
        }

        static Lazy<T> val(std::function<T*(void)> supplier) {
            return Lazy<T>(supplier);
        }

        T* Get() & {
            if (m_underlying != nullptr) {
                return m_underlying;
            }

            m_underlying = m_supplier();
            PG_CORE_ASSERT(m_underlying != nullptr, "Lazy initialization failed!");

            return m_underlying;
        }
    private:
        Lazy(std::function<T*(void)> supplier) {
            m_supplier = supplier;
        }

        T* m_underlying = nullptr;
        std::function<T*(void)> m_supplier = []() { return nullptr; };
    };
}
