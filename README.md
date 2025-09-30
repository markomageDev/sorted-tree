# SortedTree

A simple **thread-safe in-memory sorted tree** implemented in C++ using a custom AVL tree.

## Features

- Supports **put** and **get** methods only.
- Keys and values can be **any type**; for byte arrays, use `std::vector<uint8_t>`.
- Thread-safe using `std::mutex`.
- Custom AVL tree implementation (no library data structures used).

## Why This Data Structure?

AVL tree is used because:

- Keeps keys sorted automatically.
- Provides O(log n) insertions and lookups.
- Balances itself to maintain performance.
- Ideal for `get` and `put` operations.

## Template Syntax

`SortedTree<KeyType, ValueType>` allows you to create a sorted tree for any key and value types.

**Example using byte arrays (`Bytes`):**

```cpp
#include "SortedTree.h"
#include <vector>
#include <optional>
#include <iostream>

using Bytes = std::vector<uint8_t>;

// Create a tree where both keys and values are byte arrays
SortedTree<Bytes, Bytes> tree;

Bytes key = {1, 2, 3};
Bytes value = {10, 20, 30};
tree.put(key, value);

auto val = tree.get(key);
if(val) {
    // Print value
    for(auto b : *val) std::cout << (int)b << " ";
    std::cout << "\n";
}
