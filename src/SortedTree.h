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
        Node(const Key& k, const Value& v, Node * parent = nullptr)
            : key(k), value(v), left(nullptr), right(nullptr), parent(parent), height(1) {}
        ~Node()=default;

        bool operator < (const Node& n) const {
            return key < n.key;
        }

        Key key;
        Value value;
        Node* left;
        Node* right;
        Node* parent;
        int height;
    };

    Node* root;

    void deleteTree(Node* node);
    void insertRecursively(Node * curr, const Key& key, const Value& value);
};

template <typename Key, typename Value>
SortedTree<Key, Value>::SortedTree() { root = nullptr; }

template <typename Key, typename Value>
SortedTree<Key, Value>::~SortedTree() { deleteTree(root); };

template<typename Key, typename Value>
void SortedTree<Key, Value>::deleteTree(Node* node) {
    if(!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

template <typename Key, typename Value>
void SortedTree<Key, Value>::put(const Key& key, const Value& value) {
    if(!root) root = new Node(key, value);
    else insertRecursively(root, key, value);
}

template<typename Key, typename Value>
void SortedTree<Key, Value>::insertRecursively(Node * curr, const Key& key, const Value& value) {
    if(curr->key == key) {
        curr->value = value;
        return;
    }

    if(key < curr->key) {
        if(!curr->left) {
            curr->left = new Node(key, value, curr);
        }
        else insertRecursively(curr->left, key, value);
    }
    else {
        if(!curr->right) {
            curr->right = new Node(key, value, curr);
        }
        else insertRecursively(curr->right, key, value);
    }
}

template <typename Key, typename Value>
std::optional<Value> SortedTree<Key, Value>::get(const Key& key) const {
    Node * curr = root;
    while(curr) {
        if(curr->key == key) return curr->value;
        if(key < curr->key) curr = curr->left;
        else curr = curr->right;
    }
    return std::nullopt;
}