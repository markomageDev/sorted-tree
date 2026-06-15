cmake_minimum_required(VERSION 3.20)

project(algorithms-library LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(algorithms-library
		library_tests/AVLtreeTest.cpp
		libraries/AVLtree.h)

