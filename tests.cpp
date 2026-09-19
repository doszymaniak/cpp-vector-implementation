#include "myvector.hpp"
#include <cassert>
#include <gtest/gtest.h>
#include <stdexcept>
#include <utility>

struct TrackingValue {
  static int constructed;
  static int destroyed;
  static int alive;

  static void reset() {
    constructed = 0;
    destroyed = 0;
    alive = 0;
  }

  int value;

  TrackingValue() : value(0) {
    ++constructed;
    ++alive;
  }

  explicit TrackingValue(int v) : value(v) {
    ++constructed;
    ++alive;
  }

  TrackingValue(const TrackingValue &other) : value(other.value) {
    ++constructed;
    ++alive;
  }

  TrackingValue(TrackingValue &&other) noexcept : value(other.value) {
    ++constructed;
    ++alive;
    other.value = 0;
  }

  TrackingValue &operator=(const TrackingValue &other) {
    if (this != &other) {
      value = other.value;
    }
    return *this;
  }

  TrackingValue &operator=(TrackingValue &&other) noexcept {
    if (this != &other) {
      value = other.value;
      other.value = 0;
    }
    return *this;
  }

  ~TrackingValue() {
    ++destroyed;
    --alive;
  }
};

int TrackingValue::constructed = 0;
int TrackingValue::destroyed = 0;
int TrackingValue::alive = 0;

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

TEST(MyVectorLifetimeTest, ClearDestroysAllLiveTrackedObjects) {
  TrackingValue::reset();
  MyVector<TrackingValue> values;

  values.push_back(TrackingValue{1});
  values.push_back(TrackingValue{2});
  values.push_back(TrackingValue{3});

  EXPECT_EQ(TrackingValue::alive, 3);

  const int destroyed_before_clear = TrackingValue::destroyed;
  values.clear();

  EXPECT_TRUE(values.empty());
  EXPECT_EQ(TrackingValue::alive, 0);
  EXPECT_GE(TrackingValue::destroyed, destroyed_before_clear + 3);
}

TEST(MyVectorLifetimeTest, ResizeShrinksByDestroyingExtraObjects) {
  TrackingValue::reset();
  MyVector<TrackingValue> values;

  values.push_back(TrackingValue{10});
  values.push_back(TrackingValue{20});
  values.push_back(TrackingValue{30});
  values.push_back(TrackingValue{40});

  EXPECT_EQ(TrackingValue::alive, 4);

  const int destroyed_before_resize = TrackingValue::destroyed;
  values.resize(2, TrackingValue{0});

  EXPECT_EQ(values.size(), 2);
  EXPECT_EQ(TrackingValue::alive, 2);
  EXPECT_GE(TrackingValue::destroyed, destroyed_before_resize + 2);
}

TEST(MyVectorLifetimeTest, CopyConstructorDuplicatesTrackedObjects) {
  TrackingValue::reset();
  MyVector<TrackingValue> original;
  original.push_back(TrackingValue{7});
  original.push_back(TrackingValue{8});

  EXPECT_EQ(TrackingValue::alive, 2);

  MyVector<TrackingValue> copy(original);

  EXPECT_EQ(copy.size(), 2);
  EXPECT_EQ(copy[0].value, 7);
  EXPECT_EQ(copy[1].value, 8);
  EXPECT_EQ(TrackingValue::alive, 4);
}