cmake_minimum_required(VERSION 3.16)
project(sorted_tree LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Test executable ----
add_executable(test_sorted
        test_sorted.cpp
)

# Add include directory so test can see src/SortedTree.h
target_include_directories(test_sorted PRIVATE src)

# ---- Enable testing ----
enable_testing()
add_test(NAME test_sorted COMMAND test_sorted)