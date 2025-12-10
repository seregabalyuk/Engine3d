#pragma once

#include "Empty.h"
#include <stddef.h>

namespace traits {
  template<size_t I>
  struct Get;

  template<>
  struct Get<0> {
    template<class T, class... Args>
    constexpr static auto&& get(T&& val, Args&&... ) {
      return std::forward<T>(val);
    }

    constexpr static auto&& get() {
      return empty;
    }
  };

  template<size_t I>
  struct Get {
    template<class T, class... Args>
    constexpr static auto&& get(T&&, Args&&... args) {
      return Get<I - 1>::get(std::forward<Args>(args)...);
    }

    constexpr static auto&& get() {
      return empty;
    }
  };

  template<size_t I, class...Args>
  constexpr auto&& get(Args&&... args) {
    return Get<I>::get(std::forward<Args>(args)...);
  }

}