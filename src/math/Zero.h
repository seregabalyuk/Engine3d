#pragma once

#include <utility>


namespace math { 
  struct Zero {};

  template<class U>
  constexpr U&& operator+(Zero zero, U&& other) {
    return std::forward<U>(other);
  }

  template<class U>
  constexpr U&& operator+(U&& other, Zero zero) {
    return std::forward<U>(other);
  }

  template<class U>
  constexpr Zero operator*(Zero zero, U&& other) {
    return Zero();
  }

  template<class U>
  constexpr Zero operator*(U&& other, Zero zero) {
    return Zero();
  }
} // namespace math