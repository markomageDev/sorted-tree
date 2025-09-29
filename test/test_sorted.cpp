#include "../src/SortedTree.h"
#include <iostream>
#include <cassert>
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

void testLeftRightHeavyInsert() {
    SortedTree<int, int> tree;

    // Right-heavy insert (ascending order)
    for (int i = 1; i <= 5; i++) tree.put(i, i*i);
    for (int i = 1; i <= 5; i++) assert(tree.get(i) == i*i);

    // Left-heavy insert (descending order)
    SortedTree<int, int> tree2;
    for (int i = 5; i >= 1; i--) tree2.put(i, i*i);
    for (int i = 5; i >= 1; i--) assert(tree2.get(i) == i*i);
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
    testLeftRightHeavyInsert();
    testDuplicateKeys();

    std::cout << "All assertions passed" << std::endl;
    return 0;
}