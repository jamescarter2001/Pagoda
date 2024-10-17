#pragma once
#include "pgpch.h"

namespace Pagoda::Base {
    template <typename K, typename V>
    class Map {
    public:
        virtual inline void Put(const K key, const V value) = 0;
        virtual inline void Remove(const K& key) = 0;

        virtual inline const V* FindFirst(const std::function<bool(const K&, const V&)>& biPredicate) {
            for (const K& key : Keys()) {
                const V& value = Get(key);
                if (biPredicate(key, value)) {
                    return &value;
                }
            }

            return nullptr;
        }

        virtual inline const V& Get(const K& key) const = 0;
        virtual inline const std::vector<K>& Keys() const = 0;
        virtual inline bool Contains(const K& key) const = 0;
    };
}