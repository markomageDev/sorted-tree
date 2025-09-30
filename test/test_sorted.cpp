#include "../src/SortedTree.h"
#include <iostream>
#include <cassert>
#include <cstdint>
#include <vector>

using Bytes = std::vector<uint8_t>;

void testBasicPutGet() {
    SortedTree<int, std::string> tree;

    // Insert some nodes
    tree.put(4, "A");
    tree.put(2, "B");
    tree.put(6, "C");
    tree.put(1, "D");
    tree.put(3, "E");

    // Basic checks
    assert(tree.get(4) == "A");
    assert(tree.get(3) == "E");
    assert(tree.get(6) == "C");

    // Update existing key
    tree.put(4, "Change");
    assert(tree.get(4) == "Change");

    // Missing key
    assert(tree.get(10) == std::nullopt);

    // Edge cases
    SortedTree<int, std::string> emptyTree;
    assert(emptyTree.get(0) == std::nullopt);

    SortedTree<int, std::string> singleNodeTree;
    singleNodeTree.put(42, "Only");
    assert(singleNodeTree.get(42) == "Only");
}

void testAVLRotations() {
    // LL case
    {
        SortedTree<int, int> tree;
        tree.put(30, 30);
        tree.put(20, 20);
        tree.put(10, 10); // triggers LL -> single right rotation
        assert(tree.get(30).has_value());
        assert(tree.get(20).has_value());
        assert(tree.get(10).has_value());
    }

    // RR case
    {
        SortedTree<int, int> tree;
        tree.put(10, 10);
        tree.put(20, 20);
        tree.put(30, 30); // triggers RR -> single left rotation
        assert(tree.get(10).has_value());
        assert(tree.get(20).has_value());
        assert(tree.get(30).has_value());
    }

    // LR case
    {
        SortedTree<int, int> tree;
        tree.put(30, 30);
        tree.put(10, 10);
        tree.put(20, 20); // triggers LR -> left rotation + right rotation
        assert(tree.get(10).has_value());
        assert(tree.get(20).has_value());
        assert(tree.get(30).has_value());
    }

    // RL case
    {
        SortedTree<int, int> tree;
        tree.put(10, 10);
        tree.put(30, 30);
        tree.put(20, 20); // triggers RL -> right rotation + left rotation
        assert(tree.get(10).has_value());
        assert(tree.get(20).has_value());
        assert(tree.get(30).has_value());
    }
}

void testDuplicateKeys() {
    SortedTree<int, std::string> tree;
    tree.put(1, "A");
    tree.put(1, "B");  // update
    tree.put(1, "C");  // update again
    assert(tree.get(1) == "C");
}

int main() {
    testBasicPutGet();
    testAVLRotations();
    testDuplicateKeys();

    std::cout << "All assertions passed" << std::endl;
    return 0;
}