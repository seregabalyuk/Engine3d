#pragma once
#include <cstddef>

namespace geom::matrix {
  template<class T, size_t N, size_t M>
  struct DefaultContainer {
    T array[N * M];
    DefaultContainer() {}
    DefaultContainer(const T& value) {
      for (auto& cell: array) {
        cell = value;
      }
    }

    T& operator()(size_t i, size_t j) {
      return array[i * M  + j];
    }

    const T& operator()(size_t i, size_t j) const {
      return array[i * M  + j];
    }
  };
} // namespace geom::matrix