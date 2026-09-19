#ifndef MYVECTOR_HPP
#define MYVECTOR_HPP

#include <cstddef>
#include <initializer_list>
#include <memory>

template <typename T> class MyVector {
private:
  using allocator_type = std::allocator<T>;
  using allocator_traits = std::allocator_traits<allocator_type>;

  allocator_type allocator;
  std::size_t m_size;
  std::size_t m_capacity;
  T *vec;

  T *allocate(std::size_t cap);
  void destroy_elements() noexcept;
  void deallocate() noexcept;
  void reallocate(std::size_t min_cap);

public:
  // Constructors
  MyVector(std::size_t cap);
  MyVector();
  MyVector(std::initializer_list<T> l);
  MyVector(const MyVector &other);
  MyVector(MyVector &&other);

  // Destructor
  ~MyVector();

  // Operators
  MyVector &operator=(const MyVector &other);
  MyVector &operator=(MyVector &&other);
  T &operator[](std::size_t idx);
  const T &operator[](std::size_t idx) const;

  // Other
  void push_back(const T &el);
  void pop_back();
  void reserve(std::size_t n);
  void resize(std::size_t n, const T &val);
  T &at(std::size_t idx);
  const T &at(std::size_t idx) const;
  std::size_t size() const;
  std::size_t capacity() const;
  bool empty() const;
  void clear();

  // Iterators
  T *begin();
  const T *begin() const;
  T *end();
  const T *end() const;
};

#include "myvector.tpp"

#endif