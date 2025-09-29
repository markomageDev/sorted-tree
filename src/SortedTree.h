#pragma once
#include <optional>
#include <shared_mutex>

template <typename Key, typename Value>
class SortedTree {
public:
    SortedTree();
    ~SortedTree();

    // disable copying for now
    SortedTree(const SortedTree&) = delete;
    SortedTree& operator=(const SortedTree&) = delete;

    // Insert or replace value by key
    void put(const Key& key, const Value& value);

    // Get value by key (std::nullopt if not found)
    std::optional<Value> get(const Key& key) const;

private:
    // filler: not implemented yet
    mutable std::shared_mutex mtx_;
};

template <typename Key, typename Value>
SortedTree<Key, Value>::SortedTree() = default;

template <typename Key, typename Value>
SortedTree<Key, Value>::~SortedTree() = default;

template <typename Key, typename Value>
void SortedTree<Key, Value>::put(const Key& key, const Value& value) {
    // TODO: implement later
    (void)key;   // suppress unused variable warning
    (void)value;
}

template <typename Key, typename Value>
std::optional<Value> SortedTree<Key, Value>::get(const Key& key) const {
    // TODO: implement later
    (void)key;
    return std::nullopt;
}