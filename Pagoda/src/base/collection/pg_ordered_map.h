#pragma once
#include "pgpch.h"
#include "pg_map.h"

namespace Pagoda::Base {
    template <typename K, typename V>
    class OrderedMap : public Map<K, V> {
    public:
        virtual inline void Put(const K key, const V value) override {
            m_underlying.insert({key, value});
            m_insertionOrder.emplace_back(key);
        }

        virtual inline void Remove(const K& key) override {
            m_underlying.erase(key);
            m_insertionOrder.erase(std::find(m_insertionOrder.begin(), m_insertionOrder.end(), key));
        }

        virtual inline const V& Get(const K& key) const override {
            return m_underlying.at(key);
        }

        virtual inline const std::vector<K>& Keys() const override {
            return m_insertionOrder;
        }

        virtual inline bool Contains(const K& key) const override {
            return m_underlying.find(key) != m_underlying.end();
        }

    private:
        std::vector<K> m_insertionOrder;
        std::unordered_map<K, V> m_underlying;
    };
}