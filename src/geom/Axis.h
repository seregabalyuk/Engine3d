#pragma once

#include <stddef.h>
#include <utility>
#include "../traits/Empty.h"


namespace geom {
  template<class T, size_t N>
  class Axis {
    T value;
   public:
    constexpr Axis(): value() {}
    constexpr Axis(traits::Empty): value() {
      static_assert(false, "Few argumnts");
    }
    constexpr Axis(const T& other):value(other) {}
    constexpr Axis(T&& other):value(std::move(other)) {}
    constexpr T& back() { return value; }
    constexpr const T& back() const { return value; }
    
  };

  template<class T>
  struct Axis<T, 1> { 
    T x;
    constexpr Axis(): x() {}
    constexpr Axis(traits::Empty): x() {
      static_assert(false, "Few argumnts");
    }
    constexpr Axis(const T& other):x(other) {}
    constexpr Axis(T&& other):x(std::move(other)) { }
    constexpr T& back() { return x; }
    constexpr const T& back() const { return x; }
  };

  template<class T>
  struct Axis<T, 2> { 
    T y;
    constexpr Axis(): y() {}
    constexpr Axis(traits::Empty): y() {
      static_assert(false, "Few argumnts");
    }
    constexpr Axis(const T& other):y(other) {}
    constexpr Axis(T&& other):y(std::move(other)) { }
    constexpr T& back() { return y; }
    constexpr const T& back() const { return y; }
  };

  template<class T>
  struct Axis<T, 3> { 
    T z;
    constexpr Axis(): z() {}
    constexpr Axis(traits::Empty): z() {
      static_assert(false, "Few argumnts");
    }
    constexpr Axis(const T& other):z(other) {}
    constexpr Axis(T&& other):z(std::move(other)) { }
    constexpr T& back() { return z; }
    constexpr const T& back() const { return z; }
  };

  template<class T>
  struct Axis<T, 4> { 
    T t;
    constexpr Axis(): t() {}
    constexpr Axis(traits::Empty): t() {
      static_assert(false, "Few argumnts");
    }
    constexpr Axis(const T& other):t(other) {}
    constexpr Axis(T&& other):t(std::move(other)) { }
    constexpr T& back() { return t; }
    constexpr const T& back() const { return t; }
  };
}