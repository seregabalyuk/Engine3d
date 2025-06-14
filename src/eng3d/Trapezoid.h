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
      auto&& context
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

      auto pixels_y = surface.ptr() + _y_begin * surface.width;
      context.setY(_y_begin);

      for (
        int y = _y_begin; 
        y < _y_end; 
        ++ y, 
        pixels_y += surface.width,
        context.incY()
      ) {
        int _x_begin = std::max((int)left->x(y) , 0);
        int _x_end = std::min((int)right->x(y), surface.width);
        auto it_begin = pixels_y + _x_begin;
        auto it_end = pixels_y + _x_end;
        context.setX(_x_begin);
        for (
          auto it = it_begin; 
          it < it_end; 
          ++ it, 
          context.incX()
        ) {
          *it = context.getColor();
        }
        #ifdef DEBUG_TRAPEZOID
          *it_begin = COLOR(0, 0, 255);
          *(it_end - 1) = COLOR(0, 0, 255);
        #endif
      }

      #ifdef DEBUG_TRAPEZOID_LONGLINES
      {
        auto pixels_from = surface.ptr() + _y_begin * surface.width;
        auto pixels_to = pixels_from + surface.width;
        for(auto it = pixels_from; it < pixels_to; ++ it)
          *it = COLOR(0, 255, 0);

        pixels_from = surface.ptr() + _y_end * surface.width;
        pixels_to = pixels_from + surface.width;
        for(auto it = pixels_from; it < pixels_to; ++ it)
          *it = COLOR(0, 0, 255);
      }
      #endif

      #ifdef DEBUG_TRAPEZOID
      {
        int _x_begin = std::max((int)left->x(_y_begin) , 0);
        int _x_end = std::min((int)right->x(_y_begin), surface.width);
        auto pixels_from = surface.ptr() + 
          _y_begin * surface.width;
        auto pixels_to = pixels_from + _x_end;
        pixels_from += _x_begin;
        for(auto it = pixels_from; it < pixels_to; ++ it)
          *it = COLOR(0, 0, 255);
        
        _x_begin = std::max((int)left->x(_y_end - 1) , 0);
        _x_end = std::min((int)right->x(_y_end - 1), surface.width);
        pixels_from = surface.ptr() + 
          (_y_end - 1) * surface.width;
        pixels_to = pixels_from + _x_end;
        pixels_from += _x_begin;
        for(auto it = pixels_from; it < pixels_to; ++ it)
          *it = COLOR(0, 0, 255);
      }
      #endif
    }
  };

  
  
} // namespace eng3d