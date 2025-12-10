#pragma once
#include "Declaration.h"


namespace geom {
  template<class T, size_t N>
  struct Ray {
    Vector<T, N> position;
    Vector<T, N> direction;
  };
} // namespace geom