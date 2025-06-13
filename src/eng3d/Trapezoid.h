#pragma once
#include "../geom/Line.h"
#include "Surface.h"

#include <algorithm>

namespace eng3d {
  template<class T>
  struct Trapezoid {
    using Line = geom::Line<T, 2>;
    
    int y_begin;
    int y_end;
    const Line* left;
    const Line* right;

    Trapezoid(
      int y_begin, 
      int y_end, 
      const Line& left,
      const Line& right
    ): y_begin(y_begin),
      y_end(y_end),
      left(&left),
      right(&right)
    {}

    template<class Pixel>
    void draw(
      Surface<Pixel>& surface,
      auto func
    ) const {
      int _y_begin = std::max(0, y_begin);
      int _y_end = std::min(y_end, surface.height);
      // cut y by left line
      if (left->B != 0) { 
        int _y_per = left->y(surface.width - 1);
        if (left->B < 0) {
          _y_end = std::min(_y_per, _y_end);
        } else {
          _y_begin = std::max(_y_per, _y_begin);
        }
      } else if (left->C >= surface.width * left-> A) {
        return;
      }

      // cut y by rigth line
      if (right->B != 0) { 
        int _y_per = right->y(0);
        if (right->B > 0) {
          _y_end = std::min(_y_per, _y_end);
        } else {
          _y_begin = std::max(_y_per, _y_begin);
        }
      } else if (right->C < 0) {
        return;
      }
      if (_y_begin >= _y_end) return;

      for (int y = _y_begin; y < _y_end; ++ y) {
        int _x_begin = std::max((int)left->x(y) , 0);
        int _x_end = std::min((int)right->x(y), surface.width);
        for (int x = _x_begin; x < _x_end; ++ x) {
          surface(x, y) = func(x, y);
        }
      }

      #ifdef DEBUG_TRAPEZOID
      for(int x = 0; x < surface.width; ++ x)
        surface(x, _y_begin) = COLOR(0, 255, 0);
      for(int x = 0; x < surface.width; ++ x)
        surface(x, _y_end) = COLOR(0, 0, 255);
      #endif
    }
  };

  
  
} // namespace eng3d