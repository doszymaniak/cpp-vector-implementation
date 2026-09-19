#include "myvector.hpp"
#include <cassert>
#include <iostream>

void test_push_back() {
  MyVector<int> v1({1, 2, 3, 4});
  assert(v1.size() == 4);
  v1.push_back(5);
  v1.push_back(6);
  v1.push_back(7);
  assert(v1.size() == 7);
  assert(v1[4] == 5);
}

void test_copy_constructor() {
  MyVector<int> v1({5, 6, 7});
  MyVector<int> v2(v1);
  assert(v2.size() == 3);
  assert(v2[0] == 5);
  assert(v2[1] == 6);
  assert(v2[2] == 7);
}

void test_copy_assignment() {
  MyVector<int> v1({6, 7, 8});
  MyVector<int> v2;
  v2 = v1;
  assert(v2.size() == 3);
  assert(v2[0] == 6);
  assert(v2[1] == 7);
  assert(v2[2] == 8);
}

void test_pop_back() {
  MyVector<int> v1({6, 7});
  v1.pop_back();
  assert(v1.size() == 1);
  assert(v1[0] == 6);
  v1.pop_back();
  assert(v1.empty());
  v1.pop_back(); // empty vector - no exception
}

void test_reserve() {
  MyVector<int> v1;
  v1.reserve(6);
  assert(v1.capacity() >= 6);
}

void test_resize() {
  MyVector<int> v1 = {1, 2, 3};
  v1.resize(14, -1);
  for (size_t i = 3; i < 14; i++)
    assert(v1[i] == -1);
  assert(v1.size() == 14);
}

void test_empty() {
  MyVector<int> v1;
  assert(v1.empty());
  v1.push_back(5);
  assert(!v1.empty());
}

void test_clear() {
  MyVector<int> v1 = {5, 8, 8};
  v1.clear();
  assert(v1.empty());
}

void test_iterator() {
  MyVector<int> v1 = {1, 2, 3, 4, 5};
  auto *start = v1.begin();
  assert(*start == 1);
  auto *end = v1.end();
  assert(*(end - 1) == 5);
}

int main() {
  test_push_back();
  test_copy_constructor();
  test_copy_assignment();
  test_pop_back();
  test_reserve();
  test_resize();
  test_empty();
  test_clear();
  test_iterator();
  std::cout << "ALL TESTS PASSED!\n";
  return 0;
}