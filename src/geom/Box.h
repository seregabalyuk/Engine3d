#pragma once
#include "Declaration.h"


namespace geom {
  template<class T, size_t N>
  struct Box {
    Vector<T, N> from;
    Vector<T, N> size;
  };
} // namespace geom