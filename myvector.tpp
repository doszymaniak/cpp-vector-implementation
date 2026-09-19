#include <stdexcept>

template <typename T> T *MyVector<T>::allocate(std::size_t cap) {
  if (cap == 0)
    return nullptr;
  return allocator_traits::allocate(allocator, cap);
}

template <typename T> void MyVector<T>::destroy_elements() noexcept {
  while (m_size > 0) {
    --m_size;
    allocator_traits::destroy(allocator, vec + m_size);
  }
}

template <typename T> void MyVector<T>::deallocate() noexcept {
  if (vec != nullptr) {
    allocator_traits::deallocate(allocator, vec, m_capacity);
  }
  vec = nullptr;
  m_capacity = 0;
}

template <typename T>
MyVector<T>::MyVector(std::size_t cap)
    : allocator(), m_size(0), m_capacity(cap), vec(allocate(cap)) {}

template <typename T>
MyVector<T>::MyVector()
    : allocator(), m_size(0), m_capacity(10), vec(allocate(10)) {}

template <typename T>
MyVector<T>::MyVector(std::initializer_list<T> l)
    : allocator(), m_size(0), m_capacity(l.size()), vec(allocate(l.size())) {

  try {
    for (const T &el : l) {
      allocator_traits::construct(allocator, vec + m_size, el);
      ++m_size;
    }
  } catch (...) {
    destroy_elements();
    deallocate();
    throw;
  }
}

template <typename T>
MyVector<T>::MyVector(const MyVector &other)
    : allocator(), m_size(0), m_capacity(other.m_capacity),
      vec(allocate(other.m_capacity)) {
  try {
    for (std::size_t i = 0; i < other.m_size; i++) {
      allocator_traits::construct(allocator, vec + m_size, other.vec[i]);
      ++m_size;
    }
  } catch (...) {
    destroy_elements();
    deallocate();
    throw;
  }
}

template <typename T>
MyVector<T>::MyVector(MyVector &&other)
    : allocator(), m_size(other.m_size), m_capacity(other.m_capacity),
      vec(other.vec) {
  other.vec = nullptr;
  other.m_capacity = 0;
  other.m_size = 0;
}

template <typename T> MyVector<T>::~MyVector() {
  destroy_elements();
  deallocate();
}

template <typename T>
MyVector<T> &MyVector<T>::operator=(const MyVector &other) {
  if (this == &other)
    return *this;

  T *new_vec = allocate(other.m_capacity);

  try {
    for (std::size_t i = 0; i < other.m_size; ++i) {
      allocator_traits::construct(allocator, new_vec + i, other.vec[i]);
    }
  } catch (...) {
    for (std::size_t i = 0; i < other.m_size; ++i) {
      allocator_traits::destroy(allocator, new_vec + i);
    }
    allocator_traits::deallocate(allocator, new_vec, other.m_capacity);
    throw;
  }

  destroy_elements();
  deallocate();

  vec = new_vec;
  m_capacity = other.m_capacity;
  m_size = other.m_size;
  return *this;
}

template <typename T> MyVector<T> &MyVector<T>::operator=(MyVector &&other) {
  if (this == &other)
    return *this;

  destroy_elements();
  deallocate();

  vec = other.vec;
  m_capacity = other.m_capacity;
  m_size = other.m_size;

  other.vec = nullptr;
  other.m_capacity = 0;
  other.m_size = 0;
  return *this;
}

template <typename T> void MyVector<T>::reallocate(std::size_t min_cap) {
  std::size_t new_capacity = (m_capacity == 0) ? 10 : m_capacity * 2;
  if (new_capacity < min_cap)
    new_capacity = min_cap;

  std::size_t old_size = m_size;
  T *old_vec = vec;
  std::size_t old_capacity = m_capacity;

  T *new_vec = allocate(new_capacity);

  try {
    for (std::size_t i = 0; i < old_size; ++i) {
      allocator_traits::construct(allocator, new_vec + i, old_vec[i]);
    }
  } catch (...) {
    for (std::size_t i = 0; i < old_size; ++i) {
      allocator_traits::destroy(allocator, new_vec + i);
    }
    allocator_traits::deallocate(allocator, new_vec, new_capacity);
    throw;
  }

  for (std::size_t i = 0; i < old_size; ++i) {
    allocator_traits::destroy(allocator, old_vec + i);
  }

  if (old_vec != nullptr) {
    allocator_traits::deallocate(allocator, old_vec, old_capacity);
  }

  vec = new_vec;
  m_capacity = new_capacity;
  m_size = old_size;
}

template <typename T> void MyVector<T>::push_back(const T &el) {
  if (m_size == m_capacity)
    reallocate(m_size + 1);
  allocator_traits::construct(allocator, vec + m_size, el);
  ++m_size;
}

template <typename T> void MyVector<T>::pop_back() {
  if (m_size == 0)
    return;
  --m_size;
  allocator_traits::destroy(allocator, vec + m_size);
}

template <typename T> void MyVector<T>::reserve(std::size_t n) {
  if (m_capacity >= n)
    return;
  reallocate(n);
}

template <typename T> void MyVector<T>::resize(std::size_t n, const T &val) {
  if (n < m_size) {
    while (m_size > n) {
      --m_size;
      allocator_traits::destroy(allocator, vec + m_size);
    }
    return;
  }

  if (n > m_capacity)
    reallocate(n);

  while (m_size < n) {
    allocator_traits::construct(allocator, vec + m_size, val);
    ++m_size;
  }
}

template <typename T> T &MyVector<T>::operator[](std::size_t idx) {
  return vec[idx];
}

template <typename T> const T &MyVector<T>::operator[](std::size_t idx) const {
  return vec[idx];
}

template <typename T> T &MyVector<T>::at(std::size_t idx) {
  if (idx >= m_size)
    throw std::out_of_range("Index out of range!");
  return vec[idx];
}

template <typename T> const T &MyVector<T>::at(std::size_t idx) const {
  if (idx >= m_size)
    throw std::out_of_range("Index out of range!");
  return vec[idx];
}

template <typename T> std::size_t MyVector<T>::size() const { return m_size; }

template <typename T> std::size_t MyVector<T>::capacity() const {
  return m_capacity;
}

template <typename T> bool MyVector<T>::empty() const { return (m_size == 0); }

template <typename T> void MyVector<T>::clear() { destroy_elements(); }

template <typename T> T *MyVector<T>::begin() { return vec; }

template <typename T> const T *MyVector<T>::begin() const { return vec; }

template <typename T> T *MyVector<T>::end() { return vec + m_size; }

template <typename T> const T *MyVector<T>::end() const { return vec + m_size; }