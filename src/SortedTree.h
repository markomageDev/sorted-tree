#pragma once
#include <optional>
#include <shared_mutex>

template <typename Key, typename Value>
class SortedTree {
public:
    SortedTree();
    ~SortedTree();

    SortedTree(const SortedTree&) = delete;
    SortedTree& operator=(const SortedTree&) = delete;

    void put(const Key& key, const Value& value);
    std::optional<Value> get(const Key& key) const;

private:
    struct Node {
        Node(const Key& k, const Value& v)
            : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
        ~Node()=default;

        Key key;
        Value value;
        Node* left;
        Node* right;
        int height;
    };

    Node* root;

    void DeleteTree(Node* node);
};

template <typename Key, typename Value>
SortedTree<Key, Value>::SortedTree() { root = nullptr; }

template <typename Key, typename Value>
SortedTree<Key, Value>::~SortedTree() { DeleteTree(root); };

template<typename Key, typename Value>
void SortedTree<Key, Value>::DeleteTree(Node* node) {
    if(node == nullptr) return;
    DeleteTree(node->left);
    DeleteTree(node->right);
    delete node;
}

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