#include "../src/SortedTree.h"
#include <iostream>
#include <cassert>
#include <vector>

using Bytes = std::vector<uint8_t>;

int main() {
    SortedTree<Bytes, Bytes> tree;

    Bytes key = { 'a', 'b', 'c' };
    Bytes value = { 1, 2, 3 };

    tree.put(key, value);
    auto result = tree.get(key);

    // Filler: currently always nullopt
    assert(!result.has_value());

    std::cout << "Initial test ran successfully (filler implementation)" << std::endl;
    return 0;
}