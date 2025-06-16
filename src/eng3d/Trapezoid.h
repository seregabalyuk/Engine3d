#pragma once
#include "../geom/Line.h"

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

    template<class Surf>
    void draw(
      Surf& surface,
      auto&& context
    ) const {
      int _y_begin = std::max(0, y_begin);
      int _y_end = std::min(y_end, (int)surface.height);
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

      auto surfCon = surface.context();
      surfCon.setY(_y_begin);
      context.setY(_y_begin);

      for (
        int y = _y_begin; 
        y < _y_end; 
        ++ y, 
        surfCon.incY(),
        context.incY()
      ) {
        int _x_begin = std::max((int)left->x(y) , 0);
        int _x_end = std::min((int)right->x(y), (int)surface.width);
        context.setX(_x_begin);
        surfCon.setX(_x_begin);
        for (
          int x = _x_begin; 
          x < _x_end; 
          ++ x,
          context.incX(),
          surfCon.incX()
        ) {
          surfCon.set(context);
        }
        #ifdef DEBUG_TRAPEZOID
        if (_x_begin < _x_end) {
          *surface.ptr(_x_begin, y) = COLOR(0, 0, 255);
          *surface.ptr(_x_end - 1, y) = COLOR(0, 0, 255);
        }
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
        int _x_end = std::min((int)right->x(_y_begin), (int)surface.width);
        auto pixels_from = surface.ptr() + 
          _y_begin * surface.width;
        auto pixels_to = pixels_from + _x_end;
        pixels_from += _x_begin;
        for(auto it = pixels_from; it < pixels_to; ++ it)
          *it = COLOR(0, 0, 255);
        
        _x_begin = std::max((int)left->x(_y_end - 1) , 0);
        _x_end = std::min((int)right->x(_y_end - 1), (int)surface.width);
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