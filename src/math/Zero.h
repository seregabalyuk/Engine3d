#pragma once

#include <utility>


namespace math { 
  struct Zero {};

  template<class U>
  U&& operator+(Zero zero, U&& other) {
    return std::forward<U>(other);
  }

  template<class U>
  U&& operator+(U&& other, Zero zero) {
    return std::forward<U>(other);
  }

  template<class U>
  Zero operator*(Zero zero, U&& other) {
    return Zero();
  }

  template<class U>
  Zero operator*(U&& other, Zero zero) {
    return Zero();
  }
} // namespace math