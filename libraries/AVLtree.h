#pragma once
#include <algorithm>
#include <optional>
#include <mutex>
#include <vector>


template <typename Key, typename Value>
class AVLtree
{
public:
    AVLtree() = default;
    ~AVLtree();

    AVLtree(const AVLtree&) = delete;
    AVLtree& operator=(const AVLtree&) = delete;

    void put(const Key& key, const Value& value);

    std::optional<Value> get(const Key& key) const;

    bool remove(const Key& key);

    bool contains(const Key& key) const;

    int size() const {
        std::lock_guard lock(tree_mutex);
        return node_size(root);
    }

    bool is_empty() const {
        std::lock_guard lock(tree_mutex);
        return !root;
    }

    std::vector<std::pair<Key, Value>> in_order() const {
        std::lock_guard lock(tree_mutex);
        return in_order_node(root);
    }

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

    Node* root = nullptr;

    mutable std::mutex tree_mutex;

    void delete_cascade(Node* node);

    static Node* find_min(Node* node);

    void transplant(Node* u, Node* v);

    static int height(Node* node) { return node ? node->height : 0; }

    void right_rotation(Node*& y);

    void left_rotation(Node*& y);

    void balance(Node*& curr);

    void update_height(Node*& n) const { if (n) n->height = std::max(height(n->left), height(n->right)) + 1; }

    int node_size(Node* node) const {
        if (!node) return 0;
        return node_size(node->left) + node_size(node->right) + 1;
    }

    std::vector<std::pair<Key, Value>> in_order_node(Node* node) const;
};

template <typename Key, typename Value>
AVLtree<Key, Value>::~AVLtree() { delete_cascade(root); }

template<typename Key, typename Value>
void AVLtree<Key, Value>::delete_cascade(Node* node) {
    if(!node) return;
    delete_cascade(node->left);
    delete_cascade(node->right);
    delete node;
}

template<typename Key, typename Value>
void AVLtree<Key,Value>::right_rotation(Node*& y) {
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
    update_height(oldY);
    update_height(x);

    y = x;  // reassign parent's pointer
}

template<typename Key, typename Value>
void AVLtree<Key,Value>::left_rotation(Node*& y) {
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

    update_height(oldY);
    update_height(x);

    y = x;
}

template<typename Key, typename Value>
typename AVLtree<Key, Value>::Node* AVLtree<Key, Value>::find_min(Node* node) {
    if (!node) return nullptr;

    while (node->left) node = node->left;
    return node;
}

template<typename Key, typename Value>
void AVLtree<Key, Value>::transplant(Node* u, Node* v) {
    if (!u->parent) {
        root = v;
    }
    else if (u->parent->left == u) {
        u->parent->left = v;
    }
    else {
        u->parent->right = v;
    }

    if (v) v->parent = u->parent;
}

template<typename Key, typename Value>
void AVLtree<Key, Value>::balance(Node*& curr) {
    while(curr){
        int balance = height(curr->left) - height(curr->right);

        if(balance > 1) {
            int children_balance = height(curr->left->left) - height(curr->left->right);
            if(children_balance >= 0)
                right_rotation(curr);
            else {
                left_rotation(curr->left);
                right_rotation(curr);
            }
        }
        else if (balance < -1) {
            int children_balance = height(curr->right->left) - height(curr->right->right);
            if(children_balance <= 0)
                left_rotation(curr);
            else {
                right_rotation(curr->right);
                left_rotation(curr);
            }
        }

        update_height(curr);
        curr = curr->parent;
    }
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

    balance(curr);
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

template <typename Key, typename Value>
bool AVLtree<Key, Value>::contains(const Key& key) const {
    std::lock_guard lock(tree_mutex);

    Node * curr = root;
    while (curr) {
        if (curr->key == key) return true;

        if (key < curr->key) curr = curr->left;
        else curr = curr->right;
    }

    return false;
}

template <typename Key, typename Value>
bool AVLtree<Key, Value>::remove(const Key& key) {
    std::lock_guard lock(tree_mutex);

    Node * curr = root;
    while (curr && curr->key != key) {
        curr = key < curr->key ? curr->left : curr->right;
    }

    if (!curr) return false;

    if (curr->left && curr->right) {
        Node* successor = find_min(curr->right);
        curr->key = successor->key;
        curr->value = successor->value;
        curr = successor;
    }

    Node* child = curr->left ? curr->left : curr->right;
    Node* rebalance_from = curr->parent;

    transplant(curr, child);
    delete curr;

    if (rebalance_from) {
        balance(rebalance_from);
    }

    return true;
}

template <typename Key, typename Value>
std::vector<std::pair<Key, Value>> AVLtree<Key, Value>::in_order_node(Node* node) const {

    if (!node) return std::vector<std::pair<Key, Value>>();

    auto in_order_left = in_order_node(node->left);
    auto in_order_right = in_order_node(node->right);

    in_order_left.push_back({node->key, node->value});
    in_order_left.insert(in_order_left.end(), in_order_right.begin(), in_order_right.end());

    return in_order_left;
}