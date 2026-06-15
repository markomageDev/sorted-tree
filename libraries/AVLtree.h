#pragma once
#include <optional>
#include <mutex>
#include <vector>


template <typename Key, typename Value>
class AVLtree
{
public:
    AVLtree();
    ~AVLtree();

    AVLtree(const AVLtree&) = delete;
    AVLtree& operator=(const AVLtree&) = delete;

    void put(const Key& key, const Value& value);

    std::optional<Value> get(const Key& key) const;

    bool remove(const Key& key);

    bool contains(const Key& key) const;

    int size() const;

    bool is_empty() const;

    std::vector<std::pair<Key, Value>> in_order() const;

    std::vector<std::pair<Key, Value>> range_query() const;

private:
    struct Node
    {
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

    mutable std::mutex tree_mutex;

    void deleteCascade(Node* node);

    static int height(Node* node) { return node ? node->height : 0; }

    void rightRotation(Node*& y);

    void leftRotation(Node*& y);

    void updateHeight(Node*& n) const { if (n) n->height = std::max(height(n->left), height(n->right)) + 1; }
};

template <typename Key, typename Value>
AVLtree<Key, Value>::AVLtree() { root = nullptr; }

template <typename Key, typename Value>
AVLtree<Key, Value>::~AVLtree() { deleteCascade(root); };

template<typename Key, typename Value>
void AVLtree<Key, Value>::deleteCascade(Node* node) {
    if(!node) return;
    deleteCascade(node->left);
    deleteCascade(node->right);
    delete node;
}

template<typename Key, typename Value>
void AVLtree<Key,Value>::rightRotation(Node*& y) {
    Node* oldY = y;
    Node* x = y->left;
    Node* middle = x->right;

    x->right = oldY;
    oldY->left = middle;

    x->parent = oldY->parent;
    oldY->parent = x;
    if (middle) middle->parent = oldY;

    if (!x->parent)
        root = x;
    else if (x->parent->left == oldY) {
        x->parent->left = x;
    }
    else {
        x->parent->right = x;
    }

    // update heights
    updateHeight(oldY);
    updateHeight(x);

    y = x;  // reassign parent's pointer
}

template<typename Key, typename Value>
void AVLtree<Key,Value>::leftRotation(Node*& y) {
    Node* oldY = y;
    Node* x = y->right;
    Node* middle = x->left;

    x->left = oldY;
    oldY->right = middle;

    x->parent = oldY->parent;
    oldY->parent = x;
    if (middle) middle->parent = oldY;

    if (!x->parent)
        root = x;
    else if (x->parent->left == oldY) {
        x->parent->left = x;
    }
    else {
        x->parent->right = x;
    }

    updateHeight(oldY);
    updateHeight(x);

    y = x;
}

template <typename Key, typename Value>
void AVLtree<Key, Value>::put(const Key& key, const Value& value) {
    std::lock_guard lock(tree_mutex); // lock while modifying

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
        int balance = height(curr->left) - height(curr->right);

        if(balance > 1) {
            int children_balance = height(curr->left->left) - height(curr->left->right);
            if(children_balance >= 0)
                rightRotation(curr);
            else {
                leftRotation(curr->left);
                rightRotation(curr);
            }
        }
        else if (balance < -1) {
            int children_balance = height(curr->right->left) - height(curr->right->right);
            if(children_balance <= 0)
                leftRotation(curr);
            else {
                rightRotation(curr->right);
                leftRotation(curr);
            }
        }

        updateHeight(curr);
        curr = curr->parent;
    }
}

template <typename Key, typename Value>
std::optional<Value> AVLtree<Key, Value>::get(const Key& key) const {
    std::lock_guard lock(tree_mutex); // lock while reading

    Node * curr = root;
    while(curr) {
        if(curr->key == key) return curr->value;
        if(key < curr->key) curr = curr->left;
        else curr = curr->right;
    }
    return std::nullopt;
}