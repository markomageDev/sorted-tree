#include "../src/SortedTree.h"
#include <iostream>
#include <cassert>
#include <cstdint>
#include <vector>

using Bytes = std::vector<uint8_t>;

void testBasicPutGet() {
    SortedTree<int, std::string> tree;

    tree.put(4, "A");
    tree.put(2, "B");
    tree.put(6, "C");
    tree.put(1, "D");
    tree.put(3, "E");

    assert(tree.get(4) == "A");
    assert(tree.get(3) == "E");
    assert(tree.get(6) == "C");

    tree.put(4, "Change");
    assert(tree.get(4) == "Change");

    assert(tree.get(10) == std::nullopt);

    SortedTree<int, std::string> emptyTree;
    assert(emptyTree.get(0) == std::nullopt);

    SortedTree<int, std::string> singleNodeTree;
    singleNodeTree.put(42, "Only");
    assert(singleNodeTree.get(42) == "Only");
}

void testAVLRotations() {
    {
        SortedTree<int, int> tree;
        tree.put(30, 30);
        tree.put(20, 20);
        tree.put(10, 10);
        assert(tree.get(30).has_value());
        assert(tree.get(20).has_value());
        assert(tree.get(10).has_value());
        assert(tree.treeHeight() == 2);
    }

    {
        SortedTree<int, int> tree;
        tree.put(10, 10);
        tree.put(20, 20);
        tree.put(30, 30);
        assert(tree.get(10).has_value());
        assert(tree.get(20).has_value());
        assert(tree.get(30).has_value());
        assert(tree.treeHeight() == 2);
    }

    {
        SortedTree<int, int> tree;
        tree.put(30, 30);
        tree.put(10, 10);
        tree.put(20, 20);
        assert(tree.get(10).has_value());
        assert(tree.get(20).has_value());
        assert(tree.get(30).has_value());
        assert(tree.treeHeight() == 2);
    }

    {
        SortedTree<int, int> tree;
        tree.put(10, 10);
        tree.put(30, 30);
        tree.put(20, 20);
        assert(tree.get(10).has_value());
        assert(tree.get(20).has_value());
        assert(tree.get(30).has_value());
        assert(tree.treeHeight() == 2);
    }

    {
        SortedTree<int, int> tree;
        for (int i = 1; i <= 7; ++i) {
            tree.put(i, i);
        }
        assert(tree.treeHeight() == 3);
    }
}

void testLargeInsertions() {
    SortedTree<int, int> tree;

    for (int i = 1; i <= 1000; ++i) {
        tree.put(i, i);
    }

    for (int i = 1; i <= 1000; ++i) {
        assert(tree.get(i).has_value());
        assert(tree.get(i).value() == i);
    }

    assert(tree.treeHeight() <= 14);

    for (int i = 1000; i >= 1; --i) {
        tree.put(i, i * 2);
    }

    for (int i = 1; i <= 1000; ++i) {
        assert(tree.get(i).value() == i * 2);
    }
}

void testDuplicateKeys() {
    SortedTree<int, std::string> tree;
    tree.put(1, "A");
    tree.put(1, "B");
    tree.put(1, "C");
    assert(tree.get(1) == "C");
}

int main() {
    testBasicPutGet();
    testAVLRotations();
    testDuplicateKeys();
    testLargeInsertions();

    std::cout << "All assertions passed" << std::endl;
    return 0;
}