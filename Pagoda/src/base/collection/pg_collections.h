#pragma once
#include "pgpch.h"
#include "base/functional/pg_functional.h"

namespace Pagoda::Base {
    class Collections {
    public:
        template <typename T>
        static const T* const FindFirst(const std::vector<T>& vec, const Predicate<T>& predicate) {
            for (const T& o : vec) {
                if (predicate(o)) {
                    return &o;
                }
            }

            return nullptr;
        }

        template<typename T>
        static std::vector<T> Filter(const std::vector<T>& vec, const Predicate<T>& predicate) {
            std::vector<T> result;
            result.reserve(vec.size());

            for (const T& o : vec) {
                if (predicate(o)) {
                    result.emplace_back(o);
                }
            }

            return result;
        }

    };
}