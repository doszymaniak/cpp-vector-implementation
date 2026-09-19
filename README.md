# cpp-vector-implementation

## Why this project
This is my custom implementation of a dynamic array in C++, inspired by std::vector from STL. I created it for learning purposes, to deepen my understanding of how a vector actually works under the hood.

It was a practical exercise in:
- raw memory allocation and deallocation
- object lifetime management
- copy and move semantics
- capacity vs size
- implementing the Rule of Five correctly

## Features
It supports the main vector-like functionality, implemented with allocator-backed raw storage:
- dynamic memory management using std::allocator and allocator_traits
- raw storage with explicit object construction and destruction
- copy constructor and move constructor
- copy assignment and move assignment
- push_back() / pop_back()
- reserve(), resize(), clear()
- element access via [] operator and at()
- iterators (begin(), end())
- GoogleTest-based validation for behavior and object lifetime

## How to build
Build and test the project using CMake:
```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```