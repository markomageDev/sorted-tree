#include <iostream>
#include <vector>
#include <thread>
#include <optional>
#include "SortedTree.h"

using Bytes = std::vector<uint8_t>;

// Helper function to print byte arrays
void printBytes(const Bytes& b) {
    for(auto byte : b) {
        std::cout << (int)byte << " ";
    }
    std::cout << "\n";
}

int main() {
    // SortedTree is a template: SortedTree<KeyType, ValueType>
    SortedTree<Bytes, Bytes> tree;  // here we use it for byte arrays

    // Lambda to insert values
    auto insertValues = [&tree](int start) {
        for(int i = 0; i < 5; ++i) {
            Bytes key = { static_cast<uint8_t>(start + i) };
            Bytes value = { static_cast<uint8_t>((start + i) * 10) };
            tree.put(key, value);  // insert into the tree
        }
    };

    // Run two threads to showcase concurrent inserts
    std::thread t1(insertValues, 0);
    std::thread t2(insertValues, 100);

    t1.join();
    t2.join();

    // Retrieve and print inserted values
    for(int i = 0; i < 105; i += 5) {
        Bytes key = { static_cast<uint8_t>(i) };
        std::optional<Bytes> value = tree.get(key);
        if(value) {
            std::cout << "Key: ";
            printBytes(key);
            std::cout << "Value: ";
            printBytes(*value);
            std::cout << "------\n";
        }
    }

    return 0;
}