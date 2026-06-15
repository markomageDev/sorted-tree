#include <iostream>
#include <vector>
#include <thread>
#include <optional>
#include <set>
#include <random>
#include <cassert>

#include "../libraries/AVLtree.h"

using namespace std;

#define TREE_SIZE 1000

void test_duplicate_put_and_missing_get() {
    AVLtree<int, string> tree;

    assert(tree.size() == 0);
    assert(tree.is_empty());

    tree.put(10, "first");
    assert(tree.size() == 1);
    tree.put(10, "second");
    assert(tree.size() == 1);

    assert(tree.get(10).has_value());
    assert(tree.get(10).value() == "second");

    assert(!tree.get(999999).has_value());
    assert(!tree.contains(999999));
}

void test_in_order() {
    AVLtree<int, string> tree;
    tree.put(3, "c");
    tree.put(8, "h");
    tree.put(1, "a");
    tree.put(9, "i");
    tree.put(2, "b");
    tree.put(4, "d");
    tree.put(10, "j");
    tree.put(5, "e");
    tree.put(6, "f");
    tree.put(7, "g");

    auto v = tree.in_order();
    assert(v.size() == 10);
    assert(v[0] == make_pair(1, string("a")));
    assert(v[1] == make_pair(2, string("b")));
    assert(v[2] == make_pair(3, string("c")));
    assert(v[3] == make_pair(4, string("d")));
    assert(v[4] == make_pair(5, string("e")));
    assert(v[5] == make_pair(6, string("f")));
    assert(v[6] == make_pair(7, string("g")));
    assert(v[7] == make_pair(8, string("h")));
    assert(v[8] == make_pair(9, string("i")));
    assert(v[9] == make_pair(10, string("j")));
}

void test_empty_tree_behavior() {
    AVLtree<int, string> empty_tree;

    assert(empty_tree.size() == 0);
    assert(empty_tree.is_empty());
    assert(!empty_tree.contains(1));
    assert(!empty_tree.get(1).has_value());
    assert(empty_tree.in_order().empty());
}

int generate_random_int(int min, int max) {
    std::random_device rd;

    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dis(min, max);

    return dis(gen);
}

bool sorted(vector<pair<int, string>>& v) {
    for (size_t i = 1; i < v.size(); i++) {
        if (v[i-1].first > v[i].first) return false;
    }
    return true;
}

void assert_in_order_equals(const AVLtree<int, string>& tree, const vector<pair<int, string>>& expected) {
    auto actual = tree.in_order();
    assert(actual == expected);
}

void test_remove_cases() {
    {
        AVLtree<int, string> tree;
        tree.put(10, "ten");
        tree.put(5, "five");
        tree.put(15, "fifteen");

        assert(tree.remove(5));
        assert(tree.size() == 2);
        assert(tree.contains(10));
        assert(tree.contains(15));
        assert(!tree.contains(5));
        assert_in_order_equals(tree, {{10, "ten"}, {15, "fifteen"}});

        assert(!tree.remove(5));
    }

    {
        AVLtree<int, string> tree;
        tree.put(10, "ten");
        tree.put(5, "five");

        assert(tree.remove(10));
        assert(tree.size() == 1);
        assert(!tree.contains(10));
        assert(tree.contains(5));
        assert_in_order_equals(tree, {{5, "five"}});
    }

    {
        AVLtree<int, string> tree;
        tree.put(20, "twenty");
        tree.put(10, "ten");
        tree.put(30, "thirty");
        tree.put(25, "twenty-five");

        assert(tree.remove(30));
        assert(tree.size() == 3);
        assert(!tree.contains(30));
        assert(tree.contains(25));
        assert_in_order_equals(tree, {{10, "ten"}, {20, "twenty"}, {25, "twenty-five"}});

        assert(tree.remove(20));
        assert(tree.size() == 2);
        assert(!tree.contains(20));
        assert(tree.contains(10));
        assert(tree.contains(25));
        assert_in_order_equals(tree, {{10, "ten"}, {25, "twenty-five"}});
    }

    {
        AVLtree<int, string> tree;
        tree.put(1, "one");
        tree.put(2, "two");
        tree.put(3, "three");

        assert(!tree.remove(99));
        assert_in_order_equals(tree, {{1, "one"}, {2, "two"}, {3, "three"}});
    }
}

void test_random() {

    AVLtree<int, string> tree;

    set<int> set;
    std::mutex set_mutex;

    auto put_routine = [&set_mutex, &set](AVLtree<int, string>& tree, size_t size) {
        for (int i = 0; i < size; i++) {
            std::lock_guard<std::mutex> lock(set_mutex);
            int random_key;

            while (true) {
                random_key = generate_random_int(0, 1000000);
                if (set.find(random_key) == set.end()) {
                    set.insert(random_key);
                    break; // break the loop only when find unique key
                }
            }
            tree.put(random_key, "Value" + std::to_string(random_key));
        }
    };

    thread t1(put_routine, std::ref(tree), TREE_SIZE / 4);
    thread t2(put_routine, std::ref(tree), TREE_SIZE / 4);
    thread t3(put_routine, std::ref(tree), TREE_SIZE / 4);
    thread t4(put_routine, std::ref(tree), TREE_SIZE / 4);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    assert(tree.size() == TREE_SIZE);
    assert(!tree.is_empty());

    auto in_order = tree.in_order();

    assert(in_order.size() == TREE_SIZE);
    assert(sorted(in_order));

    for (size_t i = 0; i < TREE_SIZE; i++) {
        assert(tree.contains(in_order[i].first));
        assert(tree.get(in_order[i].first) == in_order[i].second);
    }
}

int main() {
    test_duplicate_put_and_missing_get();
    test_in_order();
    test_empty_tree_behavior();
    test_remove_cases();
    test_random();
}