#include "myvector.hpp"
#include <cassert>
#include <gtest/gtest.h>
#include <stdexcept>
#include <utility>

TEST(MyVectorTest, PushBackAddsElements) {
  MyVector<int> values{1, 2, 3, 4};
  values.push_back(5);
  values.push_back(6);
  values.push_back(7);

  ASSERT_EQ(values.size(), 7);
  EXPECT_EQ(values[4], 5);
  EXPECT_EQ(values[6], 7);
}

TEST(MyVectorTest, CopyConstructorCopiesElements) {
  MyVector<int> original{5, 6, 7};
  MyVector<int> copy(original);

  ASSERT_EQ(copy.size(), 3);
  EXPECT_EQ(copy[0], 5);
  EXPECT_EQ(copy[1], 6);
  EXPECT_EQ(copy[2], 7);
}

TEST(MyVectorTest, CopyAssignmentCopiesElements) {
  MyVector<int> original{6, 7, 8};
  MyVector<int> copy;

  copy = original;

  ASSERT_EQ(copy.size(), 3);
  EXPECT_EQ(copy[0], 6);
  EXPECT_EQ(copy[1], 7);
  EXPECT_EQ(copy[2], 8);
}

TEST(MyVectorTest, PopBackRemovesLastElement) {
  MyVector<int> values{6, 7};
  values.pop_back();
  EXPECT_EQ(values.size(), 1);
  EXPECT_EQ(values[0], 6);

  values.pop_back();
  EXPECT_TRUE(values.empty());
  values.pop_back();
}

TEST(MyVectorTest, ReserveIncreasesCapacity) {
  MyVector<int> values;
  values.reserve(6);

  EXPECT_GE(values.capacity(), 6);
  EXPECT_TRUE(values.empty());
}

TEST(MyVectorTest, ResizeGrowsWithProvidedValue) {
  MyVector<int> values{1, 2, 3};
  values.resize(14, -1);

  ASSERT_EQ(values.size(), 14);
  for (std::size_t i = 3; i < 14; ++i) {
    EXPECT_EQ(values[i], -1);
  }
}

TEST(MyVectorTest, ResizeShrinksVector) {
  MyVector<int> values{1, 2, 3, 4};
  values.resize(2, 0);

  ASSERT_EQ(values.size(), 2);
  EXPECT_EQ(values[0], 1);
  EXPECT_EQ(values[1], 2);
}

TEST(MyVectorTest, ClearLeavesVectorEmpty) {
  MyVector<int> values{5, 8, 8};
  const std::size_t original_capacity = values.capacity();
  values.clear();

  EXPECT_TRUE(values.empty());
  EXPECT_EQ(values.capacity(), original_capacity);
}

TEST(MyVectorTest, AtThrowsForOutOfRangeIndex) {
  MyVector<int> values{1, 2, 3};

  EXPECT_THROW(values.at(3), std::out_of_range);
}

TEST(MyVectorTest, IteratorsTraverseElements) {
  MyVector<int> values{1, 2, 3, 4, 5};
  int total = 0;

  for (const int value : values) {
    total += value;
  }

  EXPECT_EQ(total, 15);
}

TEST(MyVectorTest, MoveConstructorTransfersElements) {
  MyVector<int> original{1, 2, 3};
  MyVector<int> moved(std::move(original));

  ASSERT_EQ(moved.size(), 3);
  EXPECT_EQ(moved[0], 1);
  EXPECT_TRUE(original.empty());
}