#pragma once

#include <stddef.h>
#include <utility>
#include "../traits/Empty.h"

namespace geom {
  template<class T, size_t N>
  struct Axis { 
    T back;
    Axis(): back() {}
    Axis(traits::Empty): back() {
      static_assert(false, "Few argumnts");
    }
    Axis(const T& other):back(other) {}
    Axis(T&& other):back(std::move(other)) {}
  };

  template<class T>
  struct Axis<T, 1> { 
    union { T x; T back; };
    Axis(): back() {}
    Axis(traits::Empty): back() {
      static_assert(false, "Few argumnts");
    }
    Axis(const T& other):back(other) {}
    Axis(T&& other):back(std::move(other)) {}
    ~Axis() { back.~T(); }
  };

  template<class T>
  struct Axis<T, 2> { 
    union { T y; T back; };
    Axis(): back() {}
    Axis(traits::Empty): back() {
      static_assert(false, "Few argumnts");
    }
    Axis(const T& other):back(other) {}
    Axis(T&& other):back(std::move(other)) {}
    ~Axis() { back.~T(); }
  };

  template<class T>
  struct Axis<T, 3> { 
    union { T z; T back; };
    Axis(): back() {}
    Axis(traits::Empty): back() {
      static_assert(false, "Few argumnts");
    }
    Axis(const T& other):back(other) {}
    Axis(T&& other):back(std::move(other)) {}
    ~Axis() { back.~T(); }
  };

  template<class T>
  struct Axis<T, 4> { 
    union { T t; T back; };
    Axis(): back() {}
    Axis(traits::Empty): back() {
      static_assert(false, "Few argumnts");
    }
    Axis(const T& other):back(other) {}
    Axis(T&& other):back(std::move(other)) {}
    ~Axis() { back.~T(); }
  };
}