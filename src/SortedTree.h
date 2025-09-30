#pragma once
#include <algorithm>
#include <optional>

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
        Node(const Key& k, const Value& v, Node* parent = nullptr)
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

    static int height(Node* node) { return node ? node->height : 0; }

    void rightRotation(Node*& y);

    void leftRotation(Node*& y);
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

template<typename Key, typename Value>
void SortedTree<Key,Value>::rightRotation(Node*& y) {
    Node* x = y->left;
    Node* middle = x->right;

    x->right = y;
    y->left = middle;

    x->parent = y->parent;
    y->parent = x;
    if (middle) middle->parent = y;

    if (!x->parent)
        root = x;
    else if (x->parent->left == y) {
        x->parent->left = x;
    }
    else {
        x->parent->right = x;
    }

    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;

    y = x;
}

template<typename Key, typename Value>
void SortedTree<Key,Value>::leftRotation(Node*& y) {
    Node* x = y->right;
    Node* middle = x->left;

    x->left = y;
    y->right = middle;

    x->parent = y->parent;
    y->parent = x;

    if(middle) middle->parent = y;

    if(!x->parent)
        root = x;
    else if(x->parent->left == y) {
        x->parent->left = x;
    }
    else {
        x->parent->right = x;
    }

    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;

    y = x;
}

template <typename Key, typename Value>
void SortedTree<Key, Value>::put(const Key& key, const Value& value) {
    if(!root) {
        root = new Node(key, value);
        return;
    }
    Node * curr = root;
    while(curr) {
        if(curr->key == key) {
            curr->value = value;
            return;
        }

        if(key < curr->key) {
            if(!curr->left) {
                curr->left = new Node(key, value, curr);
                break;
            }
            curr = curr->left;
        }
        else {
            if(!curr->right) {
                curr->right = new Node(key, value, curr);
                break;
            }
            curr = curr->right;
        }
    }

    while(curr){
        int leftHeight = height(curr->left);
        int rightHeight = height(curr->right);

        int balance = leftHeight - rightHeight;

        if(balance > 1) {
            rightRotation(curr);
        }
        else if (balance < -1) {
            leftRotation(curr);
        }

        curr->height = std::max(leftHeight, rightHeight) + 1;
        curr = curr->parent; // move up the tree
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