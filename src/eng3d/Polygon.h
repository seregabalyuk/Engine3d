#pragma once

#include "../geom/Vector.h"

namespace eng3d {
  template<class T, size_t N>
  struct Polygon {
    using Point = geom::Vector<T, 3>;
    
    Polygon() = default;

    

   private:
    Point _points[N];
  };
} // namespace eng3d 